<?php

/**
 * @file
 * Provides the node source plugin controller.
 */

class TMGMTNodeSourcePluginController extends TMGMTDefaultSourcePluginController {

  /**
   * Implements TMGMTSourcePluginControllerInterface::getData().
   *
   * Returns the data from the fields as a structure that can be processed by
   * the Translation Management system.
   */
  public function getData(TMGMTJobItem $job_item) {
    $node = node_load($job_item->item_id);
    $type = node_type_get_type($node);
    // Get all the fields that can be translated and arrange their values into
    // a specific structure.
    $structure = array();
    $structure['#label'] = node_type_get_name($node);
    $structure['node_title']['#label'] = $type->title_label;
    $structure['node_title']['#text'] = $node->title;
    foreach (field_info_instances('node', $node->type) as $field_name => $field_instance) {
      $info = field_info_field($field_name);
      $field_structure = module_invoke($info['module'], 'tmgmt_source_translation_structure', $field_name, $node, $info, $field_instance, $job_item);
      if ($field_structure) {
        $structure[$field_name] = $field_structure;
      }
    }
    return $structure;
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::saveTranslation().
   */
  public function saveTranslation(TMGMTJobItem $job_item) {
    if ($node = node_load($job_item->item_id)) {
      $job = $job_item->getJob();
      if (empty($node->tnid)) {
        // We have no translation source nid, this is a new set, so create it.
        $node->tnid = $node->nid;
        node_save($node);
      }
      $translations = translation_node_get_translations($node->tnid);
      if (isset($translations[$job->target_language])) {
        // We have already a translation for the source node for the target
        // language, so load it.
        $object = node_load($translations[$job->target_language]->nid);
      }
      else {
        // We don't have a translation for the source node yet, so create one.
        $object = clone $node;
        unset($object->nid, $object->vid);
        $object->language = $job->target_language;
        $object->translation_source = $node;
      }
      // Time to put the translated data into the node.
      tmgmt_node_update_node_translation($object, $job_item->translation, $job->target_language);
      // We just saved the translation, set the sate of the job item to
      // 'finished'.
      $job_item->accepted();
    }
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::getLabel().
   */
  public function getLabel(TMGMTJobItem $job_item) {
    if ($node = node_load($job_item->item_id)) {
      return entity_label('node', $node);
    }
    return parent::getLabel($job_item);
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::getUri().
   */
  public function getUri(TMGMTJobItem $job_item) {
    if ($node = node_load($job_item->item_id)) {
      return entity_uri('node', $node);
    }
    return parent::getUri($job_item);
  }

}
