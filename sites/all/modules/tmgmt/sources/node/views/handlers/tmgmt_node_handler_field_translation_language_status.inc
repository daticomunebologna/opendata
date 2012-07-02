<?php

/**
 * Field handler to display the status of all languages.
 *
 * @ingroup views_field_handlers
 */
class tmgmt_node_handler_field_translation_language_status extends views_handler_field_boolean {

  /**
   * @var views_plugin_query_default
   */
  var $query;

  /**
   * @var array
   */
  public $language_items;

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->view->init_style();
    $this->additional_fields['nid'] = 'nid';

    /**
     * Dynamically add new fields so they are used
     */
    $languages = language_list('language');
    foreach ($languages as $langcode => $lang_info) {
      $handler = views_get_handler($this->table, $this->field . '_single', 'field');
      if ($handler) {
        $id = $options['id'] . '_single_' . $langcode;
        $this->view->display_handler->handlers['field'][$id] = $handler;
        $info = array(
          'id' => $id,
          'table' => $this->table,
          'field' => $this->field . '_single',
          'label' => $lang_info->name,
        );
        $handler->langcode = $langcode;
        $handler->main_field = $options['id'];
        $handler->init($this->view, $info);
        $this->language_handlers[$langcode] = $handler;
      }
    }
  }

  function pre_render(&$values) {
    $nids = array();
    foreach ($values as $value) {
      $tnid = $this->get_value($value);
      $tnid = !empty($tnid) ? $tnid : $this->get_value($value, 'nid');
      $nids[] = $tnid;
    }
    $nodes = node_load_multiple($nids);
    $vids = array();
    foreach ($nodes as $node) {
      $vids[] = $node->vid;
    }

    $select = db_select('node', 'n');
    $select->leftJoin('tmgmt_job_item', 'tji', "n.vid = tji.item_id AND tji.plugin = 'node' AND tji.item_type = 'node'");
    $select->leftJoin('tmgmt_job', 'tj', 'tji.tjid = tj.tjid');
    $select->condition('tji.item_id', $vids);
    $select->fields('n', array('vid', 'nid'));
    $select->fields('tj', array('target_language'));
    $select->addExpression('MAX(tji.item_id)');
    $select->groupBy('n.vid');
    $select->groupBy('tj.target_language');

    $this->language_items = array();
    $result = $select->execute();
    foreach ($result as $row) {
      $this->language_items[$row->nid][$row->target_language] = 1;
    }
  }


}
