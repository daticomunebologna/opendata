<?php

/**
 * @file
 * Pages for local translation.
 */

/**
 * Menu callback. Configure which languages a user can handle.
 */
function tmgmt_local_translator_page_user_configure($account) {
  $language_list = locale_language_list('name', TRUE);
  $build['current'] = drupal_get_form('tmgmt_local_translator_user_languages_form', $account, $language_list);
  return $build;
}

/**
 * Menu callback. Translate actual job.
 */
function tmgmt_local_translator_page_job_translate(TMGMTJob $job) {
  drupal_add_css(drupal_get_path('module', 'tmgmt_local_translator') . '/tmgmt_local_translator.css');
  drupal_set_title(t('Translate %title', array('%title' => $job->defaultLabel())), PASS_THROUGH);
  $build = array();
  $build['job'] = drupal_get_form('tmgmt_local_translator_job_form', $job);
  // Build multiple forms with a wrapping fieldset for each
  foreach ($job->getItems() as $key => $item) {
    $build['items'][$key] = drupal_get_form('tmgmt_local_translator_job_item_form', $job, $item);
  }
  return $build;
}

/**
 * Job form for submitting to review.
 */
function tmgmt_local_translator_job_form($form, &$form_state, $job) {
  $form['job'] = array('#type' => 'value', '#value' => $job);
  $form['controls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Job'),
    '#description' => check_plain($job->defaultLabel()),
  );
  $languages = locale_language_list('name', TRUE);
  $states = tmgmt_job_states();
  $form['controls']['source'] = array(
    '#type' => 'item',
    '#title' => t('From'),
    '#markup' => $languages[$job->source_language],
  );
  $form['controls']['target'] = array(
    '#type' => 'item',
    '#title' => t('To'),
    '#markup' => $languages[$job->target_language],
  );
  // Now depending on state we may have different actions
  $state = $job->getState();
  $form['controls']['state'] = array(
    '#type' => 'item',
    '#title' => t('State'),
    '#markup' => $states[$state],
  );
  // @todo Check permissions and limit what we can do with this form.
  switch ($state) {
    case TMGMT_JOB_STATE_ACTIVE:
      $action = t('Submit translation');
      break;
    case TMGMT_JOB_STATE_UNPROCESSED:
      $action = t('Request translation');
      break;
    case TMGMT_JOB_STATE_REVIEW:
      $action = t('Approve translation');
      break;
  }
  if (!empty($action)) {
    $form['controls']['submit'] = array(
      '#type' => 'submit',
      '#value' => $action,
    );
  }
  return $form;
}

/**
 * Job form for submitting to review.
 *
 * @todo Proper logging, user names, etc..
 */
function tmgmt_local_translator_job_form_submit($form, &$form_state) {
  $job = $form_state['values']['job'];
  switch ($form_state['values']['op']) {
    case t('Submit translation'):
      $job->needsReview(t('The job has been translated by a local translator.'));
      drupal_set_message(t('The translation has been submitted.'));
      break;
    case t('Request translation'):
      $job->requestTranslation();
      drupal_set_message(t('The job has been submitted for translation.'));
      break;
    case t('Approve translation'):
      $job->finished(t('The translation has been approved by a local translator.'));
      drupal_set_message(t('The translation has been approved.'));
      break;
  }
}

/**
 * Provide a form to translate a job item.
 */
function tmgmt_local_translator_job_item_form($form, &$form_state, $job, $item) {
  $form['job'] = array('#type' => 'value', '#value' => $job);
  $form['job_item'] = array('#type' => 'value', '#value' => $item);
  $data = $item->getSourceData();
  $translation = $item->translation;
  $wrapper_id = 'translate-translate-job-item-' . $item->tjiid;
  $form['text'] = tmgmt_local_translator_job_item_subform($data, $translation);
  $form['text']['#prefix'] = "<div id=\"$wrapper_id\">";
  $form['text']['#suffix'] = '</div>';
  $form['text']['#type'] = 'fieldset';
  $form['text']['#collapsible'] = TRUE;
  $form['text']['controls']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save item'),
    '#ajax' => array(
      'callback' => 'tmgmt_local_translator_job_item_callback',
      'wrapper' => $wrapper_id,
      'method' => 'replace',
    ),
  );
  $form['#attributes'] = array('class' => array('tmgmt-job-item'));
  return $form;
}

/**
 * Ajax callback after saving job item
 */
function tmgmt_local_translator_job_item_callback($form, $form_state) {
  return $form['text'];
}

/**
 * Build subform for translating single text
 */
function tmgmt_local_translator_job_item_subform($items, $translation) {
  $form = array();
  $form['#tree'] = TRUE;
  if (!empty($items['#label'])) {
    $form['#title'] = $items['#label'];
    $form['#type'] = 'fieldset';
  }
  if (!empty($items['#text'])) {
    // Approximate the number of rows to use in the default textarea.
    $rows = min(ceil(str_word_count($items['#text']) / 12), 10);
    $form['source'] = array(
      '#type' => 'textarea',
      '#disabled' => TRUE,
      '#default_value' => $items['#text'],
      '#attributes' => array('class' => array('tmgmt-source')),
      '#rows' => $rows,
    );
    $form['translation'] = array(
      '#type' => 'textarea',
      '#default_value' => isset($translation['#text']) ? $translation['#text'] : '',
      '#attributes' => array('class' => array('tmgmt-translation')),
      '#rows' => $rows,
    );
  }
  if (!empty($items['#description'])) {
    $form['#description'] = $items['#description'];
  }
  foreach (element_children($items) as $key) {
    $form[$key] = tmgmt_local_translator_job_item_subform($items[$key], isset($translation[$key]) ? $translation[$key] : array());
  }
  return $form;
}

/**
 * Provide a form to translate a job item, submit.
 */
function tmgmt_local_translator_job_item_form_submit($form, &$form_state) {
  $item = $form_state['values']['job_item'];
  $translation = tmgmt_local_translator_job_item_strings($item->getSourceData(), $form_state['values']['text']);
  if ($translation) {
    $item->addTranslatedData($translation);
    //$job->needsReview();
    drupal_set_message(t('Your item translation has been saved'));
  }
}

/**
 * Recursively convert submitted strings to translation-link data arrays
 */
function tmgmt_local_translator_job_item_strings($source, $values) {
  $translation = array();
  if (isset($source['#text']) && !empty($values['translation'])) {
    $translation['#text'] = $values['translation'];
  }
  foreach (element_children($source) as $key) {
    if (isset($values[$key])) {
      $translation[$key] = tmgmt_local_translator_job_item_strings($source[$key], $values[$key]);
    }
  }
  return $translation;
}

/**
 * Form to configure which languaes user can translate
 */
function tmgmt_local_translator_user_languages_form($form, &$form_state, $account, $language_list) {
  $form['user'] = array('#type' => 'value', '#value' => $account);
  $form['language_list'] = array('#type' => 'value', '#value' => $language_list);
  if (isset($form_state['user_list'])) {
    $user_list = $form_state['user_list'];
  }
  else {
    $user_list = isset($account->tmgmt_local_translator) ? $account->tmgmt_local_translator : array();
    $user_list += array('source' => array(), 'target' => array());
    $form_state['user_list'] = $user_list;
  }
  $form['translate'] = array(
    '#prefix' => '<div id="user-languages-translate">', '#suffix' => '</div>',
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#description' => t('Please define your translation capabilities telling us which languages you can understand (Source) and into which languages you can translate (Target). You can add languages one at a time or remove existing ones.'),
  );
  // Build a generic language select element that has a blank at the end if you want to remove.
  $language_blank = array('' => '');
  $language_select = array(
    '#type' => 'select',
    '#options' => $language_list + $language_blank,
  );
  // We'll build a column for each type of languages
  foreach (array('source' => t('From'), 'target' => t('To')) as $type => $type_name) {
    $wrapper_id = 'user-languages-translate-' . $type;
    $form['translate'][$type] = array(
      '#prefix' => "<div id=\"$wrapper_id\">", '#suffix' => '</div>',
      '#type' => 'fieldset',
      '#title' => $type_name,
      '#tree' => TRUE,
    );
    // Compute available languages to be added for each category.
    $available_list = $language_list;
    foreach (array_keys($user_list[$type]) as $langcode) {
      $form['translate'][$type][$langcode]['language'] = array(
        '#type' => 'hidden',
        '#value' => $langcode,
        '#parents' => array('translate', $type, $langcode),
      );
      $form['translate'][$type][$langcode]['display'] = array(
        '#markup' => $language_list[$langcode],
      );
      $form['translate'][$type][$langcode]['remove'] = array(
        '#name' => $type . '_' . $langcode . '_remove_button',
        '#type' => 'submit',
        '#value' => t('remove'),
        '#parents' => array('controls', $type, $langcode),
        '#ajax' => array(
          'callback' => 'tmgmt_local_translator_user_ajax_callback',
          'wrapper' => $wrapper_id,
          'method' => 'replace',
        ),
        '#suffix' => '<br />',
      );
      unset($available_list[$langcode]);
    }
    // If there are any language left, add the option to add it (override options so blank is first)
    if ($available_list) {
      $form['translate'][$type]['new'] = array(
        '#type' => 'fieldset',
      );
      $form['translate'][$type]['new']['select'] = array(
        '#type' => 'select',
        '#options' => array('' => t('<more languages>')) + $available_list,
        '#default_value' => key($language_blank),
        '#parents' => array('translate', $type, 'new'),
      );
      $form['translate'][$type]['new']['add'] = array(
        '#name' => $type . '_' . $langcode . '_add_button',
        '#type' => 'submit',
        '#value' => t('add'),
        '#parents' => array('controls', $type, 'add'),
        '#ajax' => array(
          'callback' => 'tmgmt_local_translator_user_ajax_callback',
          'wrapper' => $wrapper_id,
          'method' => 'replace',
        ),
      );
    }
  }
  // To add a new target language
  $form['controls']['save'] = array('#type' => 'submit', '#value' => t('Update languages'));
  return $form;
}

/**
 * Form to configure which languaes user can translate
 */
function tmgmt_local_translator_user_languages_form_submit($form, &$form_state) {
  $account = $form_state['values']['user'];
  if (isset($form_state['values']['translate'])) {
    $user_list = array_map('array_filter', $form_state['values']['translate']);
    $user_list = array_map('array_unique', $user_list);
    $user_list = array_map('array_flip', $user_list);
  }
  else {
    $user_list = array();
  }
  $form_state['user_list'] = $user_list;
  switch ($form_state['values']['op']) {
    case t('Update languages'):
      // Save account
      $account->tmgmt_local_translator = $user_list;
      user_save($account);
      drupal_set_message(t('Your translator\'s language preferences have been saved.'));
      break;
    default:
      // Remove or add language
      $trigger = $form_state['triggering_element'];
      list($type, $langcode, $control) = explode('_', $trigger['#name']);
      if ($control == 'remove') {
        unset($form_state['user_list'][$type][$langcode]);
      }
      $form_state['rebuild'] = TRUE;
      break;
  }
}

/**
 * Ajax callback for user languages form
 */
function tmgmt_local_translator_user_ajax_callback($form, &$form_state) {
  $trigger = $form_state['triggering_element'];
  // Remove from main list, add to selector.
  list($type, $langcode) = explode('_', $trigger['#name']);
  return $form['translate'][$type];
}
