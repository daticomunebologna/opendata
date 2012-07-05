<?php

/**
 * @file
 * Provides the Supertext translation plugin controller.
 */

/**
 * Supertext translator plugin controller.
 */
class TMGMTSupertextTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Overrides TMGMTDefaultTranslatorUIController::settingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['api_account_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Supertext API Account Id'),
      '#default_value' => $translator->getSetting('api_account_id', ''),
      '#description' => t('Please enter yout supertext API Account Id.'),
    );
    $form['api_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Supertext API password'),
      '#default_value' => $translator->getSetting('api_password', ''),
      '#description' => t('Please enter your Supertext API password.'),
    );
    $form['use_sandbox'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Sandbox'),
      '#default_value' => $translator->getSetting('use_sandbox', TRUE),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }

  /**
   * Overrides TMGMTDefaultTranslatorUIController::checkoutSettingsForm().
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {
    $plugin = tmgmt_translator_plugin_controller($this->pluginType);
    $object = $plugin->generateSupertextOrderObject($job);
    $object->TargetLang = 'de-CH';
    $options = array(
      'method' => 'POST',
      'data' => json_encode($object),
      'headers' => array('Content-Type' => 'application/json'),
    );
    $response = drupal_http_request($this->sandboxURL.'/translation/quote', $options);
    if ($response->code == "200") {
      $responsedata = json_decode($response->data);
      $settings['supertext-job'] = array(
        '#type' => 'value',
      );
      $settings['job-label'] = array(
        '#type' => 'textfield',
        '#title' => t('Job Title'),
        '#default_value' => $job->label,
        '#size' => 60,
        '#maxlength' => 128,
      );
      if (isset($responsedata->Options)) {
        foreach ($responsedata->Options as $option_item) {
          $settings[$option_item->OrderTypeId . "_markup"] = array(
            '#type' => 'item',
            '#title' => $option_item->Name,
            '#markup' => $option_item->Description,
          );
          foreach ($option_item->DeliveryOptions as $delivery_key => $delivery_item) {
            $name = format_date(strtotime($delivery_item->DeliveryDate)) . ', Price ' . $delivery_item->Price . " ". $responsedata->Currency;
            $settings[$option_item->OrderTypeId .":" .$delivery_item->DeliveryId] = array(
              '#type' => 'radio',
              '#title' => $name,
              '#return_value' => $option_item->OrderTypeId .":" .$delivery_item->DeliveryId,
              '#default_value' => NULL,
              '#attributes' => array('name' => array('settings[supertext-job]')),
            );
          }
        }
      }
      return $settings;
    }
    else {
      $job->addMessage("Could not get quote from Supertext. Response: @response",array('@response'=>$response->data));
      drupal_set_message(t("Could not get quote from Supertext. Error: @error",array('@error' => $response->error)),'error');
    }
  }

}
