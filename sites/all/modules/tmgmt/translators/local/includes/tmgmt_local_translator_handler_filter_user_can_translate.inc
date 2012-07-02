<?php

/**
 * @file
 *  Provides moderation filters for Views.
 */

/**
 * Filter by whether a node type has moderation enabled or not.
 */
class tmgmt_local_translator_handler_filter_user_can_translate extends views_handler_filter_boolean_operator {

  function query() {
    if (!isset($this->value) || $this->value === NULL) {
      return;
    }
    // Check whether current user can translate the view.
    $user = user_load($GLOBALS['user']->uid);
    if ($this->value && !empty($user->tmgmt_local_translator['source']) && !empty($user->tmgmt_local_translator['target'])) {
      $source_languages = array_keys($user->tmgmt_local_translator['source']);
      $target_languages = array_keys($user->tmgmt_local_translator['target']);
      $operator = 'IN'; //($this->value ? "IN" : "NOT IN");
      $this->ensure_my_table();
      $this->query->add_where($this->options['group'], $this->table_alias . '.source_language', $source_languages, $operator);
      $this->query->add_where($this->options['group'], $this->table_alias . '.target_language', $target_languages, $operator);
    }
  }

}
