<?php

/**
 * @file
 *  Content moderation views integration for Workbench.
 */

/**
 * Implements hook_views_data().
 */
function tmgmt_local_translator_views_data() {
	$data['tmgmt_job']['tmgmt_local_user_can_translate'] = array(
    'group' => t('Translation Management Job'),
    'title' => t('User can translate'),
    'help' => t('Whether the current logged in user can translate the job.'),
    'real field' => 'source_language',
    'filter' => array(
      'handler' => 'tmgmt_local_translator_handler_filter_user_can_translate',
      'label' => t('User can translate'),
      'type' => 'yes-no',
    ), 
 );
  // Declare virtual fields on the {node} table.
  /*
  $data['node']['workbench_moderation_moderated_type'] = array(
    'group' => t('Workbench Moderation'),
    'title' => t('Type is moderated'),
    'help' => t('Whether the content type is moderated.'),
    'real field' => 'type',
    'filter' => array(
      'handler' => 'workbench_moderation_handler_filter_moderated_type',
      'label' => t('Type is moderated'),
      'type' => 'yes-no',
    ),
  );
  */
  return $data;
}
