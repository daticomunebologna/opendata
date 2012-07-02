<?php

/**
 * @file
 * Provides the Nativy Translator plugin controller.
 */

/**
 * Nativy translator plugin controller.
 */
class TMGMTNativyTranslatorPluginController extends TMGMTDefaultTranslatorPluginController {

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::isAvailable().
   */
  public function isAvailable(TMGMTTranslator $translator) {
    if ($translator->getSetting('api_user_name') && $translator->getSetting('api_password')) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::requestTranslation().
   *
   * Here we will acutally query source and get translations.
   */
  public function requestTranslation(TMGMTJob $job) {
    // This is hexadecimal order id, we'd need to store it in some field to search.
    $job->reference = $job->settings['orderid'];
    $job->submitted(t('Job submitted to Nativy for translation. You will get an e-mail when the translation is ready.'));
  }

  /**
   * Retrieve job when ready (we got an email with a link?)
   */
  public function retrieveTranslation(TMGMTJob $job) {
    if ($document = $this->nativyRetrieveTranslation($job)) {
      $translation = array();
      foreach ($document->offerrequest->textblocks as $textblock) {
        $block = $textblock->textblock;
        $code = (string) $block['code'];
        $label = (string) $block['label'];
        $text = (string) $block->description;
        $translation[$code] = array(
          '#text' => $text,
          '#label' => $label,
        );
      }
      $job->addTranslatedData(tmgmt_unflatten_data($translation));
      $job->needsReview('Got translation from Nativy');
      return TRUE;
    }
    // @todo Implement retrieve translation
    return FALSE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::getSupportedTargetLanguages().
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    // @todo: Implement this properly.
    return parent::getSupportedTargetLanguages($translator, $source_language);
  }

  /**
   * Defines plugin job settings form.
   *
   * @param $form array
   * @param $form_state array
   * @return array
   *   Settings form.
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {
    if ($iframe_url = $this->nativyCreateOffer($job)) {
      // This should look like http://test.nativy.com/client/createorder.aspx?token=a19fd9f9-4bdc-4396-aebf-277fafd9c42a&amp;orderid=f1446707-ae2f-499e-a08e-47131533a88d
      $parts = explode('orderid=', $iframe_url);
      if (count($parts) == 2 && ($orderid = $parts[1])) {
        $settings['help']['#markup'] =
          '<p>' . t('You need to complete your order through Nativy pages below and then click on <em>Submit to translator</em>.') . '</p>';

        $settings['orderid'] = array('#type' => 'value', '#value' => $orderid);
        // If we complete the order we should be at http://test.nativy.com/client/nisfinish.aspx
        // @TODO The idea is we add a hidden variable and check whether we've reached that page
        $settings['nativyurl'] = array('#type' => 'hidden', '#value' => $iframe_url);
        $settings['iframe']['#markup'] = "<iframe width=\"800\" height=\"600\" src=\"$iframe_url\"></iframe>";
        return $settings;
      }
    }
    // Something went wrong, just print message and get out.
    $settings['error']['#markup'] =
      '<p>' . t('Cannot authenticate to Nativy server. Pleaser review your user name and password.') . '</p>';
    return $settings;
  }

  /**
   * Build text as xml
   */
  protected function nativyBuildText($job) {
    $output = '';
    $template = '<textblock diffgr:id="textblock!number" msdata:rowOrder="!order" code="!code" label="!label" msdata:hiddentextblock_Id="!order">';
    $template .= '<description langCode="!langCode">';
    $template .= '!text';
    $template .= '</description>';
    $template .= "</textblock>\n";
    $data['!langCode'] = $job->source_language;
    $text = tmgmt_flatten_data($job->getSourceData());
    $number = $order = 0;
    foreach ($text as $key => $item) {
      $data['!order'] = $order;
      $data['!number'] = $number;
      $data['!code'] = $key;
      $data['!label'] = $item['#label'];
      $data['!text'] = $item['#text'];
      $output .= strtr($template, $data);
      $number++;
      $order++;
    }
    return $output;
  }

  /**
   * Create offer and return iframe URL
   */
  protected function nativyRetrieveTranslation($job) {
    $orderid = $job->reference;
    $translator = $job->getTranslator();
    $client = $this->nativySoapClient();
    // First try authentication and if successful, go for the offer.
    $authenticated = $client->authenticate($translator->getSetting('api_user_name'), $translator->getSetting('api_password'));
    if ($authenticated) {
      try {
        $response = $client->GetOffer(array('OrderId' => $orderid));
        $prefix = '<?xml version="1.0" standalone="yes"?>';
        $content = $response->GetOfferResult->any;
        // Since response is not valid XML we take out the nonsense par of it (schema)
        // Note it has nothing to do with the documented theoretical response.
        $content = array_pop(explode('</xs:schema>', $content));
        dpm($content);
        $document = new SimpleXMLElement($content);
        return $document;
      } catch (Exception $e) {
        dpm($e, 'Exception');
      }
    }
    //$content = file_get_contents(drupal_get_path('module', 'tmgmt_nativy') . '/nis_getoffer_response.xml');
    // $document = new SimpleXMLElement($content);
    // Something went wrong, just print message and get out.
    $client->printDebug();
    return FALSE;
  }

  /**
   * Create offer and return iframe URL
   */
  protected function nativyCreateOffer($job) {
    $translator = $job->getTranslator();
    $client = $this->nativySoapClient();
    // First try authentication and if successful, go for the offer.
    $authenticated = $client->authenticate($translator->getSetting('api_user_name'), $translator->getSetting('api_password'));
    if ($authenticated) {
      // Build offer parameters for straight text replacement
      // Customer data
      $data['!customerEmail'] = variable_get('site_mail', ini_get('sendmail_from'));
      $data['!customerFirstName'] = 'Drupal';
      $data['!customerLastName'] = 'Translator';
      // Languages
      $data['!primaryLangCode'] = $job->source_language; //"de-DE";
      $data['!langCode'] = $job->target_language; //"en";
      // Add text
      $data['!textblocks'] = $this->nativyBuildText($job);
      $data['!emailCallbackLink'] = url('nativy/callback', array('absolute' => TRUE));
      //$template = file_get_contents(drupal_get_path('module', 'tmgmt_nativy') . '/nis_offerrequest_sample_good.xml');
      $template = file_get_contents(drupal_get_path('module', 'tmgmt_nativy') . '/nis_offerrequest.xml');
      $template = strtr($template, $data);
      $response = $client->xmlRequest('CreateOffer', $template);
      if ($response) {
        // This should look like http://test.nativy.com/client/createorder.aspx?token=a19fd9f9-4bdc-4396-aebf-277fafd9c42a&amp;orderid=f1446707-ae2f-499e-a08e-47131533a88d
        $iframe_url = $response->CreateOfferResult;
        return $iframe_url;
      }
    }
    // Something went wrong, just print message and get out.
    $client->printDebug();
    return FALSE;
  }

  /**
   * Send request to nativy, get response
   */
  protected function nativySoapRequest($method, $params) {
    $client = $this->nativySoapClient();
    $response = $client->$method($params);
    $client->printDebug();
    return $response;
  }

  /**
   * Get SOAP client for Nativy service
   */
  protected function nativySoapClient() {
    static $client;
    if (!isset($client)) {
      $client = new TMGMTNativySoapClient(
          drupal_get_path('module', 'tmgmt_nativy') . '/tmgmt_nativy_test.wsdl',
          //drupal_get_path('module', 'tmgmt_nativy') . '/tmgmt_nativy_live.wsdl',
          array('trace' => TRUE)
      );
      // For debugging we can pull API information from the WSDL
      //dpm($client->__getFunctions(), 'Functions');
      //dpm($client->__getTypes(), 'Types');
    }
    return $client;
  }

}

/**
 * Nativy SOAP client
 */
class TMGMTNativySoapClient extends SoapClient {

  /**
   * Run request from full raw XML body
   */
  public function xmlRequest($method, $xmldata) {
    $soapvar = new SoapVar($xmldata, XSD_ANYXML);

    $response = FALSE;
    try {
      $response = $this->$method($soapvar);
    } catch (Exception $e) {
      dpm($e, 'Exception');
    }
    return $response;
  }

  /**
   * Authenticate to the service
   */
  public function authenticate($user, $pass) {
    try {
      $this->setAuthHeader(array(
        'Username' => $user,
        'Password' => $pass,
      ));
      $response = $this->AuthenticateUser();
      $token = $response->AuthenticateUserResult;
      $this->setAuthHeader(array('AuthenticatedToken' => $token));
      return TRUE;
    } catch (Exception $e) {
      $this->debug();
    }
  }

  /**
   * Set authentication header.
   */
  public function setAuthHeader($params) {
    $rawheader = '<SecuredWebServiceHeader xmlns="https://www.nativy.com/nis">';
    foreach ($params as $name => $value) {
      $rawheader .= "<$name>$value</$name>";
    }
    $rawheader .= '</SecuredWebServiceHeader>';
    $soapvar = new SoapVar($rawheader, XSD_ANYXML);
    $headers['SecuredWebServiceHeader'] = new SoapHeader(
        'http://www.nativy.com/nis',
        'SecuredWebServiceHeader',
        $soapvar
    );
    $this->__setSoapHeaders(NULL);
    $this->__setSoapHeaders($headers);
  }

  /**
   * Debug. Print out last request and response
   */
  public function printDebug() {
    dpm(array(
      'Request' => $this->__getLastRequest(),
      'RequestHeaders' => $this->__getLastRequestHeaders()
      ), 'SOAP REQUEST');
    dpm(array(
      'Response' => $this->__getLastResponse(),
      'ResponseHeaders' => $this->__getLastResponseHeaders()
      ), 'SOAP RESPONSE');
  }

}
