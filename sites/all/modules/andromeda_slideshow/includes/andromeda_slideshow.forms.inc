<?php

/**
 * @file
 * Form definitions for andromeda slideshow module
 */

/**
 * Add and edit slideshow form
 */
function andromeda_slideshow_form($form, &$form_state, $slideshow = NULL) {
  //load helper functions
  ctools_include('andromeda_slideshow', 'andromeda_slideshow');
  
  $form = array(
    '#slideshow' => isset($slideshow) ? $slideshow : andromeda_slideshow_new_slideshow(),
  );
  
  $form['title'] = array(
    '#title' => t('Title'),
    '#description' => t('Example: Front page slideshow'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($slideshow->title) ? $slideshow->title : '',
    '#attributes' => array('class' => array('slideshow-title')),
  );
  
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine-readable name'),
    '#machine_name' => array(
      'exists' => 'andromeda_slideshow_machine_name_exists',
      'source' => array('title'),
    ),
    '#description' => t('Example: front_page_slideshow') . '<br/>' . t('May only contain lowercase letters, numbers and underscores.'),
    '#required' => TRUE,
    '#default_value' => !empty($slideshow->name) ? $slideshow->name : '',
    '#attributes' => array('class' => array('slideshow-name')),
  );
  
  $form['description'] = array(
    '#title' => t('Description'),
    '#description' => t('A description for this slideshow'),
    '#type' => 'textfield',
    '#default_value' => !empty($slideshow->description) ? $slideshow->description : '',
    '#attributes' => array('class' => array('slideshow-description')),
  );
  
  $form['image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image style'),
    '#options' => andromeda_slideshow_get_image_styles(),
    '#required' => TRUE,
    '#default_value' => !empty($slideshow->settings['image_style']) ? $slideshow->settings['image_style'] : '',
  );
  
  //add slideshow types
  $types = array();
  $types['default'] = t('Default');
  foreach (andromeda_slideshow_get_types() as $key => $type) {
    $types[$key] = $type['name'];
  }
  
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => $types,
    '#required' => TRUE,
    '#default_value' => !empty($slideshow->settings['type']) ? $slideshow->settings['type'] : '',
  );
  
  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#required' => TRUE,
    '#default_value' => isset($slideshow->settings['status']) ? $slideshow->settings['status'] : 1,
  );
  
  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/slideshows'),
  );
  
  return $form;
}

/**
 * Validate handler for andromeda_slideshow_form
 */
function andromeda_slideshow_form_validate($form, &$form_state) {
  $slideshow = $form['#slideshow'];
  $machine_name = $form_state['values']['name'];
  
  if (andromeda_slideshow_machine_name_exists($machine_name)) {
    if ($machine_name != $slideshow->name) {
      form_set_error('name', t('The machine-readable name is already in use. It must be unique.'));
    }
  }
}

/**
 * Submit handler for andromeda_slideshow_form
 */
function andromeda_slideshow_form_submit($form, &$form_state) {
  $slideshow = $form['#slideshow'];
  $settings = $form_state['values'];
  $slideshow->title = check_plain($settings['title']);
  $slideshow->name = check_plain($settings['name']);
  $slideshow->description = check_plain($settings['description']);
  
  //unset some values that are not needed
  unset($settings['title'], $settings['name'], $settings['description'], $settings['submit'], $settings['form_build_id'], $settings['form_id'], $settings['form_token'], $settings['op']);
  $slideshow->settings = $settings;
  
  //save the slideshow
  $slideshow = andromeda_slideshow_save_slideshow($slideshow);
  
  if ($slideshow->sid) {
    drupal_set_message(t('Slideshow @name was saved successfully.', array(
      '@name' => $slideshow->title ,
    )));
    $form_state['redirect'] = 'admin/structure/slideshows';
  }
}

/**
 * Delete a slideshow form
 */
function andromeda_slideshow_delete_form($form, &$form_state, $slideshow) {
  $form = array(
    '#slideshow' => isset($slideshow) ? $slideshow : andromeda_slideshow_new_slideshow(),
  );
  
  $form['title'] = array(
    '#markup' => '<h2>' . t('Are you sure you want to delete the slideshow @title?', array('@title' => $slideshow->title)) . '</h2>',
  );
  
  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete')
  );
  
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/slideshows'),
  );
  
  return $form;
}

/**
 * Submit handler for andromeda_slideshow_delete_form
 */
function andromeda_slideshow_delete_form_submit($form, &$form_state) {
  $slideshow = $form['#slideshow'];
  
  //delete the slideshow
  if (andromeda_slideshow_delete_slideshow($slideshow->sid)) {
    drupal_set_message(t('Slideshow @name was deleted successfully.', array(
      '@name' => $slideshow->title,
    )));
    $form_state['redirect'] = 'admin/structure/slideshows';
  }
}

/**
 * Manage images for a slideshow form
 */
function andromeda_slideshow_manage_form($form, &$form_state, $slideshow) {
  $form = array();
  
  $images = andromeda_slideshow_load_slideshow_images($slideshow->sid);
  $form['images'] = array(
    '#theme' => 'andromeda_slideshow_manage_form_table',
    '#slideshow' => isset($slideshow) ? $slideshow : andromeda_slideshow_new_slideshow(),
    '#tree' => TRUE,
  );
  
  foreach ($images as $siid => $image) {
    $form['images'][$siid]['#image'] = $image;
    $form['images'][$siid]['position'] = array(
      '#type' => 'weight',
      '#default_value' => $image->position,
      '#attributes' => array(
        'class' => array('col-weight'),
      ),
    );
  }
  
  $form['actions'] = array();
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/slideshows'),
  );
  
  return $form;
}

/**
 * Submit handler for andromeda_slideshow_manage_form
 */
function andromeda_slideshow_manage_form_submit($form, &$form_state) {
  $slideshow = $form['images']['#slideshow'];
  $images = $form_state['values']['images'];
  
  //update the image position
  if (andromeda_slideshow_reorder_slideshow($slideshow, $images)) {
    drupal_set_message(t('Your settings have been saved.'));
  }
}

/**
 * Add and edit image form
 */
function andromeda_slideshow_image_form($form, &$form_state, $slideshow = NULL, $image = NULL) {
  $form = array(
    '#slideshow' => isset($slideshow) ? $slideshow : andromeda_slideshow_new_slideshow(),
    '#image' => isset($image) ? $image : andromeda_slideshow_new_image(),
  );
  
  $form['title'] = array(
    '#title' => t('Title'),
    '#description' => t('The title of the image'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($image->title) ? $image->title : '',
    '#attributes' => array('class' => array('image-title')),
  );
  
  $form['fid'] = array(
    '#title' => t('Image'),
    '#type' => 'managed_file',
    '#default_value' => !empty($image->fid) ? $image->fid : '',
    '#required' => TRUE,
    '#upload_validators' => array(
      'file_validate_extensions' => array('png jpg gif'),
    ),
    '#upload_location' => 'public://slideshow/images/',
  );
  
  $form['caption'] = array(
    '#title' => t('Caption'),
    '#description' => t('The image caption to use for this image.'),
    '#type' => 'textarea',
    '#default_value' => !empty($image->caption) ? $image->caption : '',
    '#attributes' => array('class' => array('image-caption')),
  );
  
  $form['link'] = array(
    '#title' => t('Link'),
    '#description' => t('Link for the image'),
    '#type' => 'textfield',
    '#default_value' => !empty($image->settings['link']) ? $image->settings['link'] : '',
    '#attributes' => array('class' => array('image-link')),
  );
  
  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/slideshows/manage/' . $slideshow->sid),
  );
  
  return $form;
}

/**
 * Submit handler for andromeda_slideshow_image_form
 */
function andromeda_slideshow_image_form_submit($form, &$form_state) {
  $settings = $form_state['values'];
  $image = $form['#image'];
  $slideshow = $form['#slideshow'];
  $slideshow_images = andromeda_slideshow_load_slideshow_images($slideshow->sid);
  
  $image->title = check_plain($settings['title']);
  $image->caption = check_plain($settings['caption']);
  
  //save the file permanently
  $file = file_load($settings['fid']);
  $file->status = FILE_STATUS_PERMANENT;  
  file_save($file);
  file_usage_add($file, 'andromeda_slideshow', 'slideshow', $slideshow->sid);
  
  $image->fid = $file->fid;
  
  //unset some values that are not needed
  unset($settings['title'], $settings['fid'], $settings['caption'], $settings['submit'], $settings['form_build_id'], $settings['form_id'], $settings['form_token'], $settings['op']);
  $image->settings = $settings;
  
  $image = andromeda_slideshow_save_image($image);
  
  if ($image->siid) {
    
    //insert image as last position
    $image->position = sizeof($slideshow_images) + 1;
    
    //add the image to this slideshow
    andromeda_slideshow_add_image($slideshow, $image);
    drupal_set_message(t('Image added to slideshow.'));
    $form_state['redirect'] = 'admin/structure/slideshows/manage/' . $slideshow->sid;
  }
}

/**
 * Delete form for a slideshow image
 */
function andromeda_slideshow_image_delete_form($form, &$form_state, $slideshow = NULL, $image = NULL) {
  $form = array(
    '#slideshow' => isset($slideshow) ? $slideshow : andromeda_slideshow_new_slideshow(),
    '#image' => isset($image) ? $image : andromeda_slideshow_new_image(),
  );
  
  $form['title'] = array(
    '#markup' => '<h2>' . t('Are you sure you want to delete the image @title?', array('@title' => $image->title)) . '</h2>',
  );
  
  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete')
  );
  
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/slideshows/manage/' . $slideshow->sid),
  );
  
  return $form;
}

/**
 * Submit handler for andromeda_slideshow_image_delete_form
 */
function andromeda_slideshow_image_delete_form_submit($form, &$form_state) {
  $image = $form['#image'];
  $slideshow = $form['#slideshow'];
  
  //remove the image from the slideshow
  if (andromeda_slideshow_remove_image($slideshow, $image)) {
    //delete the image
    if (andromeda_slideshow_delete_image($image)) {
      drupal_set_message(t('Image @name was deleted successfully.', array(
        '@name' => $image->title,
      )));
      $form_state['redirect'] = 'admin/structure/slideshows/manage/' . $slideshow->sid;
    } 
  }
}