<?php

/**
 * @todo What is this?
 */
class tmgmt_node_handler_field_translation_language_status_single extends views_handler_field_boolean {

  /**
   * @var tmgmt_node_handler_field_translation_language_status
   */
  var $main_handler;

  /**
   * @var string
   */
  var $langcode;

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['nid'] = 'nid';
  }

  function get_value($values, $field = NULL) {
    if ($field == NULL || $field == 'nid') {
      $nid = $values->nid;
      $langcode = $this->langcode;
      $value =  !empty($this->view->field[$this->main_field]->language_items[$nid][$langcode]);
      return $value;
    }
    return FALSE;
  }

  function query() {
    $this->add_additional_fields();
  }

}
