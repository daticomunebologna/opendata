<?php

/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function tmgmt_rules_action_info() {
  $info['tmgmt_rules_job_request_translation'] = array(
    'label' => t('Request Job translation'),
    'group' => t('Translation Management'),
    'parameter' => array(
      'job' => array(
        'type' => 'tmgmt_job',
        'label' => t('Translation Job'),
        'description' => t('The translation job for which translations should be requested.'),
      ),
    ),
  );
  $info['tmgmt_rules_job_accept_translation'] = array(
    'label' => t('Accept Job translation'),
    'group' => t('Translation Management'),
    'parameter' => array(
      'job' => array(
        'type' => 'tmgmt_job',
        'label' => t('Translation Job'),
        'description' => t('The translation job for which translations should be accepted.'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('An optional message'),
        'description' => t('Will be stored in the job message and displayed to the user.'),
        'optional' => TRUE,
      ),
    ),
  );
  $info['tmgmt_rules_job_cancel_translation'] = array(
    'label' => t('Cancel translation job'),
    'group' => t('Translation Management'),
    'parameter' => array(
      'job' => array(
        'type' => 'tmgmt_job',
        'label' => t('Translation Job'),
        'description' => t('The translation job that should be canceled.'),
      ),
    ),
  );
  $info['tmgmt_rules_get_job_for_language'] = array(
    'label' => t('Get a job for the defined language combination'),
    'group' => t('Translation Management'),
    'parameter' => array(
      'source_language' => array(
        'type' => 'text',
        'label' => t('Source Language'),
        'description' => t('The language from which should be translated'),
        'options list' => 'entity_metadata_language_list',
      ),
      'target_language' => array(
        'type' => 'text',
        'label' => t('Target Language'),
        'description' => t('The language into which should be translated'),
        'options list' => 'entity_metadata_language_list',
      ),
      'account' => array(
        'type' => 'user',
        'label' => t('Account'),
        'description' => t('Owner of the job'),
      ),
    ),
    'provides' => array(
      'job' => array(
        'label' => t('Job'),
        'type' => 'tmgmt_job',
      ),
    ),
  );
  $info['tmgmt_rules_job_add_item'] = array(
    'label' => t('Add an item to a job'),
    'group' => t('Translation Management'),
    'parameter' => array(
      'job' => array(
        'type' => 'tmgmt_job',
        'label' => t('Translation Job'),
        'description' => t('The translation job that should be canceled.'),
      ),
      'plugin' => array(
        'type' => 'token',
        'label' => t('Source plugin'),
        'description' => t('The source plugin of this item'),
        //'options list' => 'entity_metadata_language_list',
      ),
      'item_type' => array(
        'type' => 'token',
        'label' => t('Item type'),
        'description' => t('The item type'),
        //'options list' => 'entity_metadata_language_list',
      ),
      'item_id' => array(
        'type' => 'text',
        'label' => t('Item ID'),
        'description' => t('ID of the referenced item'),
      ),
    ),
  );
  return $info;
}

/**
 * Rules callback to request a translation of a job.
 */
function tmgmt_rules_job_request_translation(TMGMTJob $job) {
  if ($job->isTranslatable()) {
    $job->requestTranslation();
  }
}

/**
 * Rules callback to accept a translation of a job.
 */
function tmgmt_rules_job_accept_translation(TMGMTJob $job, $message) {
  foreach ($job->getItems() as $item) {
    if ($item->isNeedsReview()) {
      $item->accepted();
    }
  }
}

/**
 * Rules callback to cancel a translation job.
 */
function tmgmt_rules_job_cancel_translation(TMGMTJob $job) {
  $job->cancelTranslation();
}

/**
 * Rules callback to get the job for a specific language combination.
 */
function tmgmt_rules_get_job_for_language($source_language, $target_language, $account) {
  return array(
    'job' => tmgmt_job_match_item($source_language, $target_language, $account),
  );
}

/**
 * Rules callback to add an item to a job.
 */
function tmgmt_rules_job_add_item(TMGMTJob $job, $plugin, $item_type, $item_id) {
  $job->addItem($plugin, $item_type, $item_id);
}
