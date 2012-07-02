<?php

/**
 * @file
 * Provides page callbacks and form functions for the Translation Management
 * Tool User Interface module.
 */

/**
 * Page callback for the overview page.
 */
function tmgmt_ui_overview_page() {
  $output = array();
  if ($view = tmgmt_ui_embed_view('tmgmt_ui_job_item_review_pending')) {
    $output['review_pending'] = array(
      '#type' => 'item',
      '#title' => t('Reviews pending'),
      '#markup' => $view,
    );
  }
  if ($view = tmgmt_ui_embed_view('tmgmt_ui_job_active')) {
    $output['new_jobs'] = array(
      '#type' => 'item',
      '#title' => t('New jobs'),
      '#markup' => $view,
    );
  }
  return $output;
}

/**
 * Form callback for the settings form.
 *
 * @see system_settings_form()
 */
function tmgmt_ui_settings_form($form, &$form_state) {
  $form['workflow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow settings'),
  );
  $form['workflow']['tmgmt_quick_checkout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow quick checkout'),
    '#description' => t("Enabling this will skip the checkout form and instead directly process the translation request in cases where there is only one translator available which doesn't provide any additional configuration options."),
    '#default_value' => variable_get('tmgmt_quick_checkout', TRUE),
  );
  $form['workflow']['tmgmt_auto_accept'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow translators to automatically accept translations'),
    '#description' => t("This allows translators to optionally skip the reviewing process and automatically accept all translations as soon as they are returned by the translation provider."),
    '#default_value' => variable_get('tmgmt_auto_accept', FALSE),
  );
  $form['performance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance settings'),
  );
  $form['performance']['tmgmt_purge_finished'] = array(
    '#type' => 'select',
    '#title' => t('Purge finished jobs'),
    '#description' => t('If configured, translation jobs that have been marked as finished will be purged after a given time. The translations itself will not be deleted.'),
    '#options' => array('_never' => t('Never'), '0' => t('Immediately'), '86400' => t('After 24 hours'), '604800' => t('After 7 days'), '2592000' => t('After 30 days'), '31536000' => t('After 365 days')),
    '#default_value' => variable_get('tmgmt_purge_finished', '_never'),
  );
  return system_settings_form($form);
}

/**
 * Simple page callback for viewing a job.
 *
 * @param TMGMTJob $job
 *   The viewed job.
 *
 * @return array
 *   A renderable array.
 */
function tmgmt_ui_job_view(TMGMTJob $job) {
  return entity_view($job->entityType(), array($job), 'full', NULL, TRUE);
}

/**
 * Simple page callback for viewing a job item.
 *
 * @param TMGMTJobItem $item
 *   The viewed job item.
 *
 * @return array
 *   A renderable array.
 */
function tmgmt_ui_job_item_view(TMGMTJobItem $item) {
  // Update the breadcrumb to include the job path.
  _tmgmt_ui_breadcrumb($item);
  return entity_view($item->entityType(), array($item), 'full', NULL, TRUE);
}

/**
 * Simple page callback for reviewing a job item.
 *
 * @param TMGMTJobItem $job
 *   The job item to be reviewed
 *
 * @return array
 *   A renderable array.
 */
function tmgmt_ui_job_item_review(TMGMTJobItem $item) {
  if ($controller = $item->getSourceController()) {
    // Update the breadcrumb to include the job path.
    _tmgmt_ui_breadcrumb($item);
    $plugin = $controller->pluginType();
    return drupal_get_form('tmgmt_ui_' . $plugin . '_translation_review_form', $item);
  }
  drupal_access_denied();
  exit;
}

/**
 * Entity API form for the translator entity.
 */
function tmgmt_translator_form($form, &$form_state, TMGMTTranslator $translator, $op = 'edit') {
  // Check if the translator entity is completely new or not.
  $old = empty($translator->is_new) && $op != 'clone';
  // Build up the stored entity from the last (failed) form submit.
  if (!empty($form_state['values'])) {
    $translator = entity_ui_form_submit_build_entity($form, $form_state);
  }
  // We don't want the terrible long title that Entity API generates for us.
  if (in_array($op, array('import', 'add'))) {
    drupal_set_title($op == 'import' ? t('Import Translator') : t('Add Translator'), PASS_THROUGH);
  }
  // Check if the translator is currently in use.
  if ($busy = $old ? tmgmt_translator_busy($translator->name) : FALSE) {
    drupal_set_message(t("This translator is currently in use and therefore can't be modified or deleted."), 'warning');
  }
  $available = tmgmt_translator_plugin_labels();
  // If the translator plugin is not set, pick the first available plugin as the
  // default.
  $translator->plugin = empty($translator->plugin) ? key($available) : $translator->plugin;
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('The label of the translator.'),
    '#default_value' => $translator->label,
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 64,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#description' => t('The machine readable name of this translator. It must be unique, and it must contain only alphanumeric characters and underscores. Once created, you will not be able to change this value!'),
    '#default_value' => $translator->name,
    '#machine_name' => array(
      'exists' => 'tmgmt_translator_exists',
      'source' => array('label'),
    ),
    '#disabled' => $old,
    '#size' => 32,
    '#maxlength' => 64,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('The description of the translator.'),
    '#default_value' => $translator->description,
    '#size' => 32,
    '#maxlength' => 64,
  );
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Translator settings'),
    '#tree' => TRUE,
  );
  if (variable_get('tmgmt_auto_accept', FALSE)) {
    $form['settings']['auto_accept'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto accept finished translations'),
      '#description' => t('This skips the reviewing process and automatically accepts all translations as soon as they are returned by the translation provider.'),
      '#default_value' => $translator->getSetting('auto_accept'),
      '#weight' => -1,
    );
  }
  if (!element_children($form['settings'])) {
    unset($form['settings']);
  }
  $form['plugin_wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="tmgmt-plugin-wrapper">',
    '#suffix' => '</div>',
  );
  // Pull the translator plugin info if any.
  if ($translator->plugin) {
    $info = tmgmt_translator_plugin_info($translator->plugin);
    $form['plugin_wrapper']['plugin'] = array(
      '#type' => 'select',
      '#title' => t('Translator plugin'),
      '#description' => isset($info['description']) ? filter_xss($info['description']) : '',
      '#options' => $available,
      '#default_value' => $translator->plugin,
      '#required' => TRUE,
      '#disabled' => $busy,
      '#ajax' => array(
        'callback' => 'tmgmt_ui_ajax_callback_translator_plugin_select',
        'wrapper' => 'tmgmt-plugin-wrapper',
      ),
    );
    $form['plugin_wrapper']['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('@plugin plugin settings', array('@plugin' => $info['label'])),
      '#tree' => TRUE,
    );
    // Add the translator plugin settings form.
    $form['plugin_wrapper']['settings'] += tmgmt_ui_plugin_settings_form($form_state, $translator, $busy);
  }
  // Add a submit button and a cancel link to the form.
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save translator'),
    '#disabled' => empty($available),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('tmgmt_ui_submit_redirect'),
    '#redirect' => 'admin/config/regional/tmgmt/translators/manage/' . $translator->name . '/delete',
    '#access' => $old,
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/regional/tmgmt/translators',
  );
  return $form;
}

/**
 * Form validator for the translator entity form.
 */
function tmgmt_translator_form_validate($form, &$form_state) {
  // We need to clone this here so we don't change the translator entity in the
  // form state. Otherwise the form will break.
  $backup = clone $form_state[$form_state['entity_type']];
  $translator = entity_ui_form_submit_build_entity($form, $form_state);
  $form_state[$form_state['entity_type']] = $backup;
  if (empty($translator->plugin)) {
    form_set_error('plugin', t('You have to select a translator plugin.'));
  }
}

/**
 * Submit handler for the translator entity form.
 *
 * Populates and stores the submitted values for the translator entity via
 * entity_ui_form_submit_build_entity().
 *
 * @see entity_ui_form_submit_build_entity()
 */
function tmgmt_translator_form_submit($form, &$form_state) {
  entity_ui_form_submit_build_entity($form, $form_state)->save();
  drupal_set_message(t('The configuration options have been saved.'));
  $form_state['redirect'] = 'admin/config/regional/tmgmt/translators';
}

/**
 * Entity API form the job entity.
 */
function tmgmt_job_form($form, &$form_state, TMGMTJob $job, $op = 'edit') {
  // Get the job metadata wrapper so we can display the current status nicely.
  $wrapper = entity_metadata_wrapper('tmgmt_job', $job);
  // Set the title of the page to the label and the current state of the job
  drupal_set_title($job->label() . ' (' . $wrapper->state->label() . ')');
  // Check if the translator entity is completely new or not.
  $old = empty($job->is_new) && $op != 'clone';
  // Find pre-selected values.
  foreach (array('source_language', 'target_language') as $key) {
    // Retrieve a list of available languages per dropdown.
    $available[$key] = tmgmt_available_languages();
    // We need this information because we will lock down the language dropdown
    // fields if these values have been predefined.
    $preselected[$key] = !empty($job->{$key});
    $job->{$key} = isset($form_state['values'][$key]) ? $form_state['values'][$key] : $job->{$key};
  }
  // Remove impossible combinations so we don't end up with the same source and
  // target language in the dropdowns.
  foreach (array('source_language' => 'target_language', 'target_language' => 'source_language') as $key => $opposite) {
    if (!empty($job->{$key})) {
      unset($available[$opposite][$job->{$key}]);
    }
  }
  // Show a list of translators tagged by availability for the selected source
  // and target language combination.
  if (!$translators = tmgmt_translator_labels_flagged($job)) {
    drupal_set_message(t('There are no translators available. Before you can checkout you need to !configure at least one translator.', array('!configure' => l(t('configure'), 'admin/config/regional/tmgmt/translators'))), 'warning');
  }
  $preselected_translator = !empty($job->translator) && isset($translators[$job->translator]) ? $job->translator : key($translators);
  $job->translator = isset($form_state['values']['translator']) ? $form_state['values']['translator'] : $preselected_translator;
  $job->label = isset($form_state['values']['label']) ? $form_state['values']['label'] : $job->label;
  // Show the items that are attached to this job if there are not more than 20.
  $form['override_label'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override the default label (%label)', array('%label' => $job->label())),
    '#default_value' => !empty($job->label),
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('You can provide a label for this job in order to identify it easily later on.'),
    '#default_value' => $job->label,
    '#states' => array(
      'visible' => array(
        ':input[name="override_label"]' => array('checked' => TRUE),
      ),
    ),
  );
  if (module_exists('views') && $view = views_get_view('tmgmt_ui_job_items')) {
    $form['items'] = array(
      '#type' => 'item',
      '#title' => $view->get_title(),
      '#markup' => $view->preview('block', array($job->tjid)),
    );
  }
  $form['languages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Languages'),
    '#description' => t('The source and target language of this translation job.'),
    '#attributes' => array('class' => array('clearfix')),
  );
  // Make the source and target language flexible by showing either a select
  // dropdown or the plain string (if preselected).
  $form['languages']['source_language'] = array(
    '#title' => t('From'),
    '#type' => !empty($preselected['source_language']) || !$job->isUnprocessed() ? 'item' : 'select',
    '#options' => $available['source_language'],
    '#default_value' => $job->source_language,
    '#markup' => isset($available['source_language'][$job->source_language]) ? $available['source_language'][$job->source_language] : '',
    '#required' => TRUE,
    '#prefix' => '<div id="tmgmt-ui-source-language" class="tmgmt-ui-source-language">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'tmgmt_ui_ajax_callback_language_select',
    ),
  );
  $form['languages']['target_language'] = array(
    '#title' => t('To'),
    '#type' => !empty($preselected['target_language']) || !$job->isUnprocessed() ? 'item' : 'select',
    '#options' => $available['target_language'],
    '#default_value' => $job->target_language,
    '#markup' => isset($available['target_language'][$job->target_language]) ? $available['target_language'][$job->target_language] : '',
    '#required' => TRUE,
    '#prefix' => '<div id="tmgmt-ui-target-language" class="tmgmt-ui-target-language">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'tmgmt_ui_ajax_callback_language_select',
    ),
  );
  $form['translator_wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="tmgmt-ui-translator-wrapper">',
    '#suffix' => '</div>',
  );
  $form['translator_wrapper']['translator'] = array(
    '#type' => 'select',
    '#title' => t('Translator'),
    '#description' => t('The configured translator plugin that will process of the translation.'),
    '#options' => $translators,
    '#default_value' => $job->translator,
    '#required' => TRUE,
    '#disabled' => !$job->isUnprocessed(),
    '#ajax' => array(
      'callback' => 'tmgmt_ui_ajax_callback_translator_select',
      'wrapper' => 'tmgmt-ui-translator-settings',
    ),
  );
  // Display the checkout settings form if the job can be checked out.
  if (!empty($job->translator) && $job->isUnprocessed()) {
    $settings = tmgmt_ui_checkout_settings_form($form_state, $job);
    $form['translator_wrapper']['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Checkout settings'),
      '#prefix' => '<div id="tmgmt-ui-translator-settings">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    ) + $settings;
  }
  // Otherwise display the checkout info.
  elseif (!empty($job->translator)) {
    $form['translator_wrapper']['info'] = tmgmt_ui_checkout_info($job);
  }
  // Add the buttons and action links.
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save job'),
  );
  $form['actions']['checkout'] = array(
    '#type' => 'submit',
    '#value' => tmgmt_ui_redirect_queue_count() == 0 ? t('Submit to translator') : t('Submit to translator and continue'),
    '#access' => $job->isUnprocessed(),
    '#disabled' => empty($job->translator),
  );
  if ($old) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('tmgmt_ui_submit_redirect'),
      '#redirect' => 'admin/config/regional/tmgmt/jobs/' . $job->tjid . '/delete',
    );
  }
  // Only show the 'Cancel' button if the job has been submitted to the
  // translator.
  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#submit' => array('tmgmt_ui_submit_redirect'),
    '#redirect' => 'admin/config/regional/tmgmt/jobs',
    '#access' => $job->isActive(),
  );
  // We need some CSS for showing the source and target language in one row.
  $form['#attached']['css'][] = drupal_get_path('module', 'tmgmt_ui') . '/css/tmgmt_ui.admin.css';
  return $form;
}

/**
 * Submit callback for the job checkout form.
 */
function tmgmt_job_form_submit($form, &$form_state) {
  // Build up the job from the submitted form values and save here so we got the
  // plain 'Submit' case covered. Also, if the translator fails it won't save on
  // its own.
  $job = entity_ui_form_submit_build_entity($form, $form_state);
  if (!$form_state['values']['override_label']) {
    // Make sure the job label is empty if the override checkbox isn't checked.
    $job->label = '';
  }
  $job->save();
  // Per default we want to redirect the user to the overview.
  $form_state['redirect'] = 'admin/config/regional/tmgmt';
  // Everything below this line is only invoked if the 'Submit to translator'
  // button was clicked.
  if ($form_state['triggering_element']['#value'] == $form['actions']['checkout']['#value']) {
    if (!tmgmt_ui_job_request_translation($job)) {
      // Don't redirect the user if the translation request failed but retain
      // existing destination parameters so we can redirect once the request
      // finished successfully.
      // @todo: Change this to stay on the form in case of an error instead of
      // doing a redirect.
      $form_state['redirect'] = array(current_path(), array('query' => drupal_get_destination()));
      unset($_GET['destination']);
    }
    else if ($redirect = tmgmt_ui_redirect_queue_dequeue()) {
      // Proceed to the next redirect queue item, if there is one.
      $form_state['redirect'] = $redirect;
    }
    else {
      // Proceed to the defined destination if there is one.
      $form_state['redirect'] = tmgmt_ui_redirect_queue_destination($form_state['redirect']);
    }
  }
}

/**
 * Validation callback for the job checkout form.
 *
 * @todo Make use of the response object here.
 */
function tmgmt_job_form_validate($form, &$form_state) {
  // We need to clone this here so we don't change the job entity in the form
  // state. Otherwise the form will break.
  $backup = clone $form_state[$form_state['entity_type']];
  $job = entity_ui_form_submit_build_entity($form, $form_state);
  $form_state[$form_state['entity_type']] = $backup;
  // Load the selected translator.
  $translator = tmgmt_translator_load($job->translator);
  // Load the metadata wrapper so we can display the selected language.
  $wrapper = entity_metadata_wrapper('tmgmt_job', $job);
  // Check translator availability.
  if (!$translator->isAvailable()) {
    form_set_error('translator', $translator->getNotAvailableReason());
  }
  elseif (!$translator->canTranslate($job)) {
    form_set_error('translator', t('@translator can not translate from @source to @target.', array('@translator' => $translator->label(), '@source' => $wrapper->source_language->label(), '@target' => $wrapper->target_language->label())));
  }
}

/**
 * Helper function for retrieving the job settings form.
 *
 * @todo Make use of the response object here.
 */
function tmgmt_ui_checkout_settings_form(&$form_state, TMGMTJob $job) {
  $form = array();
  $translator = $job->getTranslator();
  if (!$translator->isAvailable()) {
    $form['#description'] = filter_xss($job->getTranslator()->getNotAvailableReason());
  }
  // @todo: if the target language is not defined, the check will not work if the first language in the list is not available.
  elseif ($job->target_language && !$translator->canTranslate($job)) {
    $wrapper = entity_metadata_wrapper('tmgmt_job', $job);
    $form['#description'] = t('@translator can not translate from @source to @target.', array('@translator' => $job->getTranslator()->label(), '@source' => $wrapper->source_language->label(), '@target' => $wrapper->target_language->label()));
  }
  else {
    $controller = tmgmt_translator_ui_controller($translator->plugin);
    $form = $controller->checkoutSettingsForm($form, $form_state, $job);
  }
  return $form;
}

/**
 * Helper function for retrieving the translator settings form.
 */
function tmgmt_ui_plugin_settings_form(&$form_state, TMGMTTranslator $translator, $busy = FALSE) {
  $form = array();
  $controller = tmgmt_translator_ui_controller($translator->plugin);
  return $controller->pluginSettingsForm($form, $form_state, $translator, $busy);
}

/**
 * Helper function for retrieving the rendered job checkout information.
 */
function tmgmt_ui_checkout_info(TMGMTJob $job) {
  $translator = $job->getTranslator();
  $controller = tmgmt_translator_ui_controller($translator->plugin);
  return $controller->checkoutInfo($job);
}

/**
 * Helper function for redirecting a form after a button has been clicked.
 */
function tmgmt_ui_submit_redirect($form, &$form_state){
  if (isset($form_state['triggering_element']['#redirect'])) {
    $form_state['redirect'] = $form_state['triggering_element']['#redirect'];
  }
}

/**
 * Ajax callback for loading the translator plugin settings form for the
 * currently selected translator plugin.
 */
function tmgmt_ui_ajax_callback_translator_plugin_select($form, &$form_state) {
  return $form['plugin_wrapper'];
}

/**
 * Ajax callback to fetch the supported translator services and rebuild the
 * target / source language dropdowns.
 */
function tmgmt_ui_ajax_callback_language_select($form, &$form_state) {
  $replace = $form_state['input']['_triggering_element_name'] == 'source_language' ? 'target_language' : 'source_language';
  $commands[] = ajax_command_replace('#tmgmt-ui-translator-wrapper', drupal_render($form['translator_wrapper']));
  $commands[] = ajax_command_replace('#tmgmt-ui-' . str_replace('_', '-', $replace), drupal_render($form['languages'][$replace]));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Ajax callback to fetch the options provided by a translator.
 */
function tmgmt_ui_ajax_callback_translator_select($form, &$form_state) {
  return $form['translator_wrapper']['settings'];
}

/**
 * Helper function for appending the parent job path to the breadcrumb of a job
 * item menu path.
 */
function _tmgmt_ui_breadcrumb(TMGMTJobItem $item) {
  $breadcrumb = drupal_get_breadcrumb();
  foreach (array('admin/config/regional/tmgmt/jobs', 'admin/config/regional/tmgmt/jobs/' . $item->tjid) as $path) {
    $menu = menu_get_item($path);
    $breadcrumb[] = l($menu['title'], $path);
  }
  drupal_set_breadcrumb($breadcrumb);
}
