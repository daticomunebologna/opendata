<?php

/**
 * @file
 * Provides the Nativy Translator ui controller.
 */

/**
 * Nativity translator ui controller.
 */
class TMGMTNativyTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['api_user_name'] = array(
      '#type' => 'textfield',
      '#title' => t('User name'),
      '#default_value' => $translator->getSetting('api_user_name', ''),
    );
    $form['api_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#default_value' => $translator->getSetting('api_password', ''),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }

}

