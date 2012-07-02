<?php

/**
 * @file
 * Provides the user translator plugin controller.
 */

class TMGMTLocalTranslatorPluginController extends TMGMTDefaultTranslatorPluginController {

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::requestTranslation().
   */
  function requestTranslation(TMGMTJob $job) {
    $job->submitted('Job queued for local translation.');
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::getSupportedTargetLanguages().
   */
  function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    $query = db_select('tmgmt_local_user', 'target')
      ->fields('target', array('language'))
      ->distinct();
    $query->join('tmgmt_local_user', 'source', 'target.uid = source.uid');
    $query->condition('source.language', $source_language);
    $list = $query->execute()->fetchCol();
    return array_flip($list);
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::checkoutInfo().
   */
  public function hasCheckoutSettings(TMGMTJob $job) {
    return FALSE;
  }

}

