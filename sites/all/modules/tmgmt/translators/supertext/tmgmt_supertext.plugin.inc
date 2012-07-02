<?php

/**
 * @file
 * Provides the Supertext translation plugin controller.
 */

/**
 * Supertext translator plugin controller.
 */
class TMGMTSupertextPluginController extends TMGMTDefaultTranslatorPluginController {

  /**
   * Translation service URL.
   *
   * @var string
   */
  protected $apiURL = 'http://www.supertext.ch/api/v1/';
  // @TODO The second one is only for testing on local network.
  protected $sandboxURL = 'http://dev.supertext.ch/api/v1/';
  //protected $sandboxURL = 'http://172.21.21.66/supertext/api/v1/';


  /**
   * Implements TMGMTTranslatorPluginControllerInterface::isAvailable().
   */
  public function isAvailable(TMGMTTranslator $translator) {
    if($translator->getSetting('api_account_id') && $translator->getSetting('api_password')) {
      return TRUE;
    }
    return FALSE;
  }

  public function generateSupertextOrderObject(TMGMTJob $job){
    $object = new stdClass();
    $object->AccountId = $job->getTranslator()->getSetting('api_account_id');
    $object->Password = $job->getTranslator()->getSetting('api_password');
    $object->CallbackUrl = url('tmgmt_supertext_callback', array('absolute' => TRUE));
    $object->CommunicationLang = "en-US";
    $object->ContentType = "text/plain";
    $object->Currency = "chf";
	  $object->Service = "translation";
    $object->SourceLang = $job->source_language;
    $object->TargetLang = $job->target_language;
    $object->ReferenceData = $job->tjid . ':' . tmgmt_supertext_hash($job->tjid);
    $object->Groups = array();

    $items = $job->getItems();
    // @todo check to use $job->getSourceData();
    foreach ($items as $tjiid => $item) {
      $itemSourceData = $item->getSourceData();
      $fields = tmgmt_flatten_data($itemSourceData);
      $group = new stdClass();
      $group->GroupId = $tjiid;
      $group->Context = $itemSourceData['#label'];
      $group->Items = array();
      foreach ($fields as $fieldarray_id => $field_array) {
        $field = new stdClass();
        $field->Content = $field_array['#text'];
        $field->Context = $field_array['#label'];
        $field->Id = $fieldarray_id;
        $group->Items[] = $field;
      }
      $object->Groups[] = $group;
    }
    return $object;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::requestTranslation().
   *
   * Here we will acutally query source and get translations.
   */
  public function requestTranslation(TMGMTJob $job) {
  	$object = $this->generateSupertextOrderObject($job);
  	$order_settings = explode(":", $job->settings['supertext-job']);
    $object->OrderTypeId = $order_settings[0];
    $object->DeliveryId = $order_settings[1];
    $job->label = $job->settings['job-label'];
	  $object->OrderTitle = $job->settings['job-label'];

    $options = array(
      'method' => 'POST',
      'data' => json_encode($object),
      'headers' => array('Content-Type' => 'application/json'),
    );

    $job->addMessage("Sending request to supertext",'debug');
    $response = drupal_http_request($this->sandboxURL.'/translation/order', $options);

    if ($response->code == "200") {
      $job->submitted("Job sent to Supertext. Response: !response",array('!response'=> $response->data));
      $responsedata = json_decode($response->data);
      // Save order id to the job
      $job->reference = $responsedata->Id;
      $job->addMessage(t("Job successfully sent to Supertext. <br />Ordertitle: @ordertitle<br />OrderID: @orderid<br />Price: @price<br />Wordcount: @wordcount<br />Deadline: @deadline",array(
        '@ordertitle' => $responsedata->OrderTitle,
        '@orderid' => $responsedata->Id,
        '@price' => $responsedata->Price,
        '@wordcount' => $responsedata->WordCount,
        '@deadline' => format_date(strtotime($responsedata->Deadline)),
      )));
    } else {
      $job->addMessage("Could not send Job to Supertext. Response: @response",array('@response'=>$response->data),'debug');
      $job->addMessage(t("Could not send Job to Supertext. Error: @error",array('@error' => $response->error)),'error');
    }
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::getSupportedTargetLanguages().
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    // @todo: Implement this properly.
    return parent::getSupportedTargetLanguages($translator, $source_language);
  }

}
