<?php

$view = new view;
$view->name = 'tmgmt_ui_job_item_messages';
$view->description = 'Lists the messages that are related to a job item.';
$view->tag = 'Translation Management';
$view->base_table = 'tmgmt_message';
$view->human_name = 'Translation Job Item Messages';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Messages';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = '';
$handler->display->display_options['style_options']['columns'] = array(
  'created' => 'created',
  'message' => 'message',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'created' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'message' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: Created */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'tmgmt_message';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['ui_name'] = 'Created';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['created']['alter']['external'] = 0;
$handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['element_label_colon'] = 1;
$handler->display->display_options['fields']['created']['element_default_classes'] = 1;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
$handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['created']['date_format'] = 'short';
/* Field: Message */
$handler->display->display_options['fields']['message']['id'] = 'message';
$handler->display->display_options['fields']['message']['table'] = 'tmgmt_message';
$handler->display->display_options['fields']['message']['field'] = 'message';
$handler->display->display_options['fields']['message']['ui_name'] = 'Message';
$handler->display->display_options['fields']['message']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['message']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['message']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['message']['alter']['external'] = 0;
$handler->display->display_options['fields']['message']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['message']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['message']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['message']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['message']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['message']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['message']['alter']['trim'] = 0;
$handler->display->display_options['fields']['message']['alter']['html'] = 0;
$handler->display->display_options['fields']['message']['element_label_colon'] = 1;
$handler->display->display_options['fields']['message']['element_default_classes'] = 1;
$handler->display->display_options['fields']['message']['hide_empty'] = 0;
$handler->display->display_options['fields']['message']['empty_zero'] = 0;
$handler->display->display_options['fields']['message']['hide_alter_empty'] = 1;
/* Sort criterion: Created */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'tmgmt_message';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['ui_name'] = 'Created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Job Item */
$handler->display->display_options['arguments']['tjiid']['id'] = 'tjiid';
$handler->display->display_options['arguments']['tjiid']['table'] = 'tmgmt_message';
$handler->display->display_options['arguments']['tjiid']['field'] = 'tjiid';
$handler->display->display_options['arguments']['tjiid']['ui_name'] = 'Job Item';
$handler->display->display_options['arguments']['tjiid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['tjiid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['tjiid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['tjiid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['tjiid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['tjiid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['tjiid']['not'] = 0;

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block');
$translatables['tmgmt_ui_job_item_messages'] = array(
  t('Master'),
  t('Messages'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Created'),
  t('Message'),
  t('All'),
  t('Block'),
);
