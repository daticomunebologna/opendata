<?php

/**
 * @file
 * Provides the user translator plugin controller.
 */

class TMGMTTestTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['expose_settings'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display settings'),
      '#default_value' => TRUE,
    );

    $form['action'] = array(
      '#type' => 'select',
      '#title' => t('Default action'),
      '#options' => array(
        'translate' => t('Translate'),
        'submit' => t('Submit'),
        'reject' => t('Reject'),
        'fail' => t('Fail'),
        'not_translatable' => t('Not translatable'),
      ),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::checkoutSettingsForm().
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {
    if ($job->getTranslator()->getSetting('expose_settings')) {
      $form['action'] = array(
        '#type' => 'select',
        '#title' => t('Action'),
        '#options' => array(
          'translate' => t('Translate'),
          'submit' => t('Submit'),
          'reject' => t('Reject'),
          'fail' => t('Fail'),
          'not_translatable' => t('Not translatable'),
        ),
        '#default_value' => $job->getTranslator()->getSetting('action'),
      );
    }
    return $form;
  }

}

class TMGMTTestTranslatorPluginController extends TMGMTDefaultTranslatorPluginController implements TMGMTTranslatorRejectDataItem {

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::hasCheckoutSettings().
   */
  public function hasCheckoutSettings(TMGMTJob $job) {
    return $job->getTranslator()->getSetting('expose_settings');
  }

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::requestTranslation().
   */
  function requestTranslation(TMGMTJob $job) {
    // Add a debug message.
    $job->addMessage('Test translator called.', array(), 'debug');

    // Do something different based on the action, if defined.
    $action = isset($job->settings['action']) ? $job->settings['action'] : '';
    switch ($action) {
      case 'submit':
        $job->submitted('Test submit.');
        break;

      case 'reject':
        $job->rejected('This is not supported.');
        break;

      case 'fail':
        // Target not reachable.
        $job->addMessage('Service not reachable.', array(), 'error');
        break;

      case 'translate':
      default:
        // The dummy translation prefixes strings with the target language.
        $data = array_filter(tmgmt_flatten_data($job->getData()), '_tmgmt_filter_data');
        $tdata = array();
        foreach ($data as $key => $value) {
          $tdata[$key]['#text'] = $job->target_language . '_' . $value['#text'];
        }
        $job->submitted('Test translation created.');
        $job->addTranslatedData(tmgmt_unflatten_data($tdata));
        break;
    }
  }

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::canTranslate().
   */
  function canTranslate(TMGMTTranslator $translator, TMGMTJob $job) {
    if (isset($job->settings['action']) && $job->settings['action'] == 'not_translatable') {
      return FALSE;
    }
    return parent::canTranslate($translator, $job);
  }

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::getSupportedTargetLanguages().
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    $languages = drupal_map_assoc(array('en', 'de', 'es', 'it', 'zh-hans'));
    unset($languages[$source_language]);
    return $languages;
  }

  /**
   * Overrides TMGMTTranslatorRejectDataItem::rejectDataItem().
   */
  public function rejectDataItem(TMGMTJobItem $job_item, array $key, array $values = NULL) {
    $key = '[' . implode('][', $key) . ']';
    $job_item->addMessage('Rejected data item @key for job item @item in job @job.', array('@key' => $key, '@item' => $job_item->tjiid, '@job' => $job_item->tjid));
    return TRUE;
  }

  /**
   * Implements TMGMTTranslatorRejectDataItem::rejectForm().
   */
  public function rejectForm($form, &$form_state) {
    return $form;
  }
}

class TMGMTTestSourcePluginController extends TMGMTDefaultSourcePluginController {

  /**
   * Overrides TMGMTDefaultSourcePluginController::getLabel().
   */
  public function getLabel(TMGMTJobItem $job_item) {
    return $this->pluginType . ':' . $job_item->item_type . ':' . $job_item->item_id;
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::getData().
   */
  public function getData(TMGMTJobItem $job_item) {
    return array(
      'dummy' => array(
        'deep_nesting' => array(
          '#text' => 'Text for job item with type ' . $job_item->item_type . ' and id ' . $job_item->item_id . '.',
          '#label' => 'Label for job item with type ' . $job_item->item_type . ' and id ' . $job_item->item_id . '.',
        ),
      ),
    );
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::saveTranslation().
   */
  public function saveTranslation(TMGMTJobItem $job_item) {
    // Set a variable that can be checked later for a given job item.
    variable_set('tmgmt_test_saved_translation_' . $job_item->item_type . '_' . $job_item->item_id, TRUE);
    $job_item->accepted();
  }
}
