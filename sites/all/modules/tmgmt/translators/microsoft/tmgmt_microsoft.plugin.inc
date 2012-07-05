<?php

/**
 * @file
 * Provides Microsoft Translator plugin controller.
 *
 * Check @link http://msdn.microsoft.com/en-us/library/dd576287.aspx Microsoft
 * Translator @endlink. Note that we are using HTTP API.
 */

/**
 * Microsoft translator plugin controller.
 */
class TMGMTMicrosoftTranslatorPluginController extends TMGMTDefaultTranslatorPluginController {

  /**
   * Translation service URL.
   *
   * @var string
   */
  protected $translatorUrl = 'http://api.microsofttranslator.com/v2/Http.svc';

  /**
   * Maximum supported characters.
   *
   * @var int
   */
  protected $maxCharacters = 10000;

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::isAvailable().
   */
  public function isAvailable(TMGMTTranslator $translator) {
    if ($translator->getSetting('api')) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::canTranslate().
   */
  public function canTranslate(TMGMTTranslator $translator, TMGMTJob $job) {
    if (!parent::canTranslate($translator, $job)) {
      return FALSE;
    }
    foreach (array_filter(tmgmt_flatten_data($job->getSourceData()), '_tmgmt_filter_data') as $value) {
      // If one of the texts in this job exceeds the max character count the job
      // can't be translated.
      if (drupal_strlen($value['#text']) > $this->maxCharacters) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::requestTranslation().
   */
  public function requestTranslation(TMGMTJob $job) {
    // Pull the source data array through the job and flatten it.
    $data = array_filter(tmgmt_flatten_data($job->getSourceData()), '_tmgmt_filter_data');
    foreach ($data as $key => $value) {
      // Query the translator API.
      $result = $this->doRequest($job->getTranslator(), 'Translate', array(
        'from' => $job->source_language,
        'to' => $job->target_language,
        'contentType' => 'text/plain',
        'text' => $value['#text'],
      ), array(
        'headers' => array(
          'Content-Type' => 'text/plain',
        ),
      ));
      // Check the result code for possible errors.
      switch ($result->code) {
        case 200:
          // Lets use DOMDocument for now because this service enables us to
          // send an array of translation sources, and we will probably use
          // this soon.
          $dom = new DOMDocument;
          $dom->loadXML($result->data);
          $items = $dom->getElementsByTagName('string');
          $data[$key]['#text'] = $items->item(0)->nodeValue;
          break;
        case 400:
          $job->rejected('Rejected by Microsoft Translator: !error', array('!error' => $result->data), 'error');
          return;
        default:
          $job->rejected('Unknown error from Microsoft Translator: !error', array('!error' => $result->data), 'error');
          return;
      }
    }
    // The translation job has been successfully submitted.
    $job->submitted('The translation job has been submitted.');
    // Save the translated data through the job.
    $job->addTranslatedData(tmgmt_unflatten_data($data));
  }

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::getSupportedTargetLanguages().
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    $languages = array();
    $request = $this->doRequest($translator, 'GetLanguagesForTranslate');
    if ($request->code == 200) {
      $dom = new DOMDocument;
      $dom->loadXML($request->data);
      foreach ($dom->getElementsByTagName('string') as $item) {
        $languages[$item->nodeValue] = $item->nodeValue;
      }
    }
    // Microsoft translator reports chinese language codes with their old names.
    // Replace them with the offical identifiers.
    if (isset($languages['zh-CHS'])) {
      unset($languages['zh-CHS']);
      $languages['zh-hans'] = 'zh-hans';
    }
    if (isset($languages['zh-CHT'])) {
      unset($languages['zh-CHT']);
      $languages['zh-hant'] = 'zh-hant';
    }
    // Check if the source language is available.
    if (array_key_exists($source_language, $languages)) {
      unset($languages[$source_language]);
      return $languages;
    }
    return array();
  }

  /**
   * Overrides TMGMTDefaultTranslatorPluginController::hasCheckoutSettings().
   */
  public function hasCheckoutSettings(TMGMTJob $job) {
    return FALSE;
  }

  /**
   * Execute a request against the Microsoft API.
   *
   * @param TMGMTTranslator $translator
   *   The translator entity to get the settings from.
   * @param $path
   *   The path that should be appended to the base uri, e.g. Translate or
   *   GetLanguagesForTranslate.
   * @param $query
   *   (Optional) Array of GET query arguments.
   * @param $options
   *   (Optional) Array of additional options passed to drupal_http_request().
   *
   * @return
   *   The response object returned by drupal_http_request().
   */
  protected function doRequest(TMGMTTranslator $translator, $path, array $query = array(), array $options = array()) {
    // fubhy, can you please stop removing this setting? This is used for the
    // tests. Thanks.
    // Answer: Okay. Will try to keep my cool :D...
    $custom_url = $translator->getSetting('url');
    $url = ($custom_url ? $custom_url : $this->translatorUrl) . '/' . $path;
    // Add the appId to the query arguments and build the query string.
    $query = array('appId' => $translator->getSetting('api')) + $query;
    $url = url($url, array('query' => $query));
    return drupal_http_request($url, $options);
  }

}
