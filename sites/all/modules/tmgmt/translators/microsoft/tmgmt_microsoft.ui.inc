<?php

/**
 * @file
 * Provides Microsoft Translator ui controller.
 */

/**
 * Microsoft translator ui controller.
 */
class TMGMTMicrosoftTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['api'] = array(
      '#type' => 'textfield',
      '#title' => t('Microsoft API Key'),
      '#default_value' => $translator->getSetting('api'),
      '#description' => t('Please enter your Microsoft API ID, or follow this !link to generate one.', array('!link' => l(t('link'),'http://www.bing.com/developers/appids.aspx'))),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator);
  }

}
