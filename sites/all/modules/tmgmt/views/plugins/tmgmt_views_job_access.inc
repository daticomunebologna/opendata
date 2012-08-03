<?php

/**
 * @file
 * Views access plugin.
 */

/**
 * Access plugin that provides role-based access control.
 */
class tmgmt_views_job_access extends views_plugin_access {

  /**
   * Overrides views_plugin_access::summary_title().
   */
  function summary_title() {
    return t('Job view access');
  }

  /**
   * Overrides views_plugin_access::access().
   */
  function access() {
    return entity_access('view', 'tmgmt_job');
  }

  /**
   * Overrides views_plugin_access::access().
   */
  function get_access_callback() {
    return array('entity_access', array('view', 'tmgmt_job'));
  }
}
