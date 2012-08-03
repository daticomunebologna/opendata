<?php

/**
 * @file
 * Provides the i18n string source controller.
 */

/**
 * Translation plugin controller for i18n strings.
 */
class TMGMTI18nStringSourcePluginController extends TMGMTDefaultSourcePluginController {

  /**
   * Overrides TMGMTDefaultSourcePluginController::getData().
   */
  public function getData(TMGMTJobItem $job_item) {
    // @todo Make use of the item type property.
    $i18n_object = explode(':', $job_item->item_id);
    $i18n_object_type = array_shift($i18n_object);
    $i18n_object_id = (count($i18n_object) == 1) ? reset($i18n_object) : $i18n_object;
    // @todo: i18n has introduced a new function and changed the existing one,
    //        conditionally call the one that exists. Remove once a new stable
    //        version is out.
    if (function_exists('i18n_get_object')) {
      $i18n_strings = i18n_get_object($i18n_object_type, $i18n_object_id)->get_strings();
    }
    else {
      $i18n_strings = i18n_object($i18n_object_type, $i18n_object_id)->get_strings();
    }
    $structure = array('#label' => 'i18n Strings: ' . $i18n_object_type);
    foreach ($i18n_strings as $string_id => $string) {
      $structure[$string_id] = array(
        '#label' => $string->title,
        '#text' => $string->string,
        '#translate' => TRUE
      );
    }
    return $structure;
  }

  /**
   * Overrides TMGMTDefaultSourcePluginController::saveTranslation().
   */
  public function saveTranslation(TMGMTJobItem $job_item) {
    $job = tmgmt_job_load($job_item->tjid);
    $data = array_filter(tmgmt_flatten_data($job_item->getData()), '_tmgmt_filter_data');
    foreach ($data as $i18n_string => $item) {
      if (isset($item['#translation']['#text'])) {
        i18n_string_translation_update($i18n_string, $item['#translation']['#text'], $job->target_language);
      }
    }
    return TRUE;
  }

}
