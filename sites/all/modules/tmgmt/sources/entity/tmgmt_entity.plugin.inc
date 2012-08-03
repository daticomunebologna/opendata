<?php

/**
 * @file
 * Provides the Entity source controller.
 */

class TMGMTEntitySourcePluginController extends TMGMTDefaultSourcePluginController {

  public function getLabel(TMGMTJobItem $job_item) {
    return entity_label($job_item->item_type, entity_load_single($job_item->item_type, $job_item->item_id));
  }

  public function getUri(TMGMTJobItem $job_item) {
    return entity_uri($job_item->item_type, entity_load_single($job_item->item_type, $job_item->item_id));
  }

  /**
   * Implements TMGMTEntitySourcePluginController::getData().
   *
   * Returns the data from the fields as a structure that can be processed by
   * the Translation Management system.
   */
  public function getData(TMGMTJobItem $job_item) {
    $entity = entity_load_single($job_item->item_type, $job_item->item_id);
    return tmgmt_field_get_source_data($job_item->item_type, $entity, TRUE);
  }

  /**
   * Implements TMGMTEntitySourcePluginController::saveTranslation().
   */
  public function saveTranslation(TMGMTJobItem $job_item) {
    $entity = entity_load_single($job_item->item_type, $job_item->item_id);
    $job = tmgmt_job_load($job_item->tjid);
    tmgmt_entity_update_entity_translation($entity, $job_item->item_type, $job_item->getData(), $job->target_language);

    $handler = entity_translation_get_handler($job_item->item_type, $entity);

    $translation = array(
      // @todo Improve hardcoded values.
      'translate' => 0,
      'status' => TRUE,
      'language' => $job_item->getJob()->target_language,
      'source' => $job_item->getJob()->source_language,
    );

    $handler->setTranslation($translation);
    $handler->saveTranslations();
    $job_item->accepted();
  }

}
