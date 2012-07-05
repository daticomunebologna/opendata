<?php

/**
 * @file
 * Admininistrative forms for Andromeda Slideshow
 */

/**
 * Slideshow admin
 */
function andromeda_slideshow_admin() {
  //load helper functions
  ctools_include('andromeda_slideshow', 'andromeda_slideshow');
  
  //get slideshows
  //load enable first
  $slideshows = andromeda_slideshow_get_enabled_slideshows();
  
  //then add disabled slideshow
  $slideshows = array_merge($slideshows, andromeda_slideshow_get_disabled_slideshows());
  
  $output = '';
  
  $table_id = 'slideshows-main-list';
  $table_classes = array('slideshow-list');
  
  $rows = array();
  $counter = 1;
  foreach ($slideshows as $sid => $slideshow) {
    
    $title = array();
    $title[] = '<div><strong>' . $slideshow->title . '</strong></div>';
    $title[] = '<div>' . t('Name : @name', array('@name' => $slideshow->name)) . '</div>';
    $title[] = '<div>' . t('Type : @type', array('@type' => $slideshow->settings['type'])) . '</div>';
    
    $links = array(
      'manage' => array(
        'title' => t('Manage'),
        'href' => 'admin/structure/slideshows/manage/' . $slideshow->sid,
      ),
      'edit' => array(
        'title' => t('Edit'),
        'href' => 'admin/structure/slideshows/edit/' . $slideshow->sid,
      ),
      'delete' => array(
        'title' => t('Delete'),
        'href' => 'admin/structure/slideshows/delete/' . $slideshow->sid,
      ),
    );
    
    //add enable and disable links
    $status_link = array();
    if ($slideshow->settings['status'] == 1) {
      $status_link['title'] = t('Disable');
      $status_link['href'] = 'admin/structure/slideshows/disable/' . $slideshow->sid;
    }
    else {
      $status_link['title'] = t('Enable');
      $status_link['href'] = 'admin/structure/slideshows/enable/' . $slideshow->sid;
    }
    
    $links['status'] = $status_link;
    
    $operations = theme('links__ctools_dropbutton', array('links' => $links));
    
    $images = andromeda_slideshow_load_slideshow_images($slideshow->sid);
    
    $row = array();
    $row[] = array('data' => implode('', $title), 'class' => array('col-title'));
    $row[] = array('data' => $slideshow->description, 'class' => array('col-description'));
    $row[] = array('data' => sizeof($images), 'class' => array('col-images'));
    $row[] = array('data' => $operations, 'class' => array('col-operations'));
    
    $rows[] = array(
      'data'  => $row,
      'class' => array(
        ($slideshow->settings['status'] == 0) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled',
      ),
    );
  }
  
  if (empty($rows)) {
    $create_link = l(t('Create a slideshow'), 'admin/structure/slideshows/create');
    $rows[] = array(array('data' => t('No slideshows created. !create.', array('!create' => $create_link)), 'colspan' => 3));
  }

  // Render the main nodequeue table.
  $header = array(t('Title'), t('Description'), t('Images'), t('Operations'));
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => $table_classes)));

  return $output;
}