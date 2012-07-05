<?php

$view = new view;
$view->name = 'tmgmt_ui_job_items';
$view->description = 'Displays all job items that belong to a job.';
$view->tag = 'Translation Management';
$view->base_table = 'tmgmt_job_item';
$view->human_name = 'Translation Job Items';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Job Items';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'label' => 'label',
  'plugin' => 'plugin',
  'state' => 'state',
  'changed' => 'changed',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'label' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'plugin' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'state' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'changed' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: Label */
$handler->display->display_options['fields']['label']['id'] = 'label';
$handler->display->display_options['fields']['label']['table'] = 'tmgmt_job_item';
$handler->display->display_options['fields']['label']['field'] = 'label';
$handler->display->display_options['fields']['label']['ui_name'] = 'Label';
$handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['label']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['label']['alter']['external'] = 0;
$handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['label']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['label']['alter']['trim'] = 0;
$handler->display->display_options['fields']['label']['alter']['html'] = 0;
$handler->display->display_options['fields']['label']['element_label_colon'] = 1;
$handler->display->display_options['fields']['label']['element_default_classes'] = 1;
$handler->display->display_options['fields']['label']['hide_empty'] = 0;
$handler->display->display_options['fields']['label']['empty_zero'] = 0;
$handler->display->display_options['fields']['label']['hide_alter_empty'] = 1;
/* Field: Plugin */
$handler->display->display_options['fields']['plugin']['id'] = 'plugin';
$handler->display->display_options['fields']['plugin']['table'] = 'tmgmt_job_item';
$handler->display->display_options['fields']['plugin']['field'] = 'plugin';
$handler->display->display_options['fields']['plugin']['ui_name'] = 'Plugin';
$handler->display->display_options['fields']['plugin']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['external'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['plugin']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['plugin']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['trim'] = 0;
$handler->display->display_options['fields']['plugin']['alter']['html'] = 0;
$handler->display->display_options['fields']['plugin']['element_label_colon'] = 1;
$handler->display->display_options['fields']['plugin']['element_default_classes'] = 1;
$handler->display->display_options['fields']['plugin']['hide_empty'] = 0;
$handler->display->display_options['fields']['plugin']['empty_zero'] = 0;
$handler->display->display_options['fields']['plugin']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['plugin']['machine_name'] = 0;
/* Field: State */
$handler->display->display_options['fields']['state']['id'] = 'state';
$handler->display->display_options['fields']['state']['table'] = 'tmgmt_job_item';
$handler->display->display_options['fields']['state']['field'] = 'state';
$handler->display->display_options['fields']['state']['ui_name'] = 'State';
$handler->display->display_options['fields']['state']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['state']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['state']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['state']['alter']['external'] = 0;
$handler->display->display_options['fields']['state']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['state']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['state']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['state']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['state']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['state']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['state']['alter']['trim'] = 0;
$handler->display->display_options['fields']['state']['alter']['html'] = 0;
$handler->display->display_options['fields']['state']['element_label_colon'] = 1;
$handler->display->display_options['fields']['state']['element_default_classes'] = 1;
$handler->display->display_options['fields']['state']['hide_empty'] = 0;
$handler->display->display_options['fields']['state']['empty_zero'] = 0;
$handler->display->display_options['fields']['state']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['state']['machine_name'] = 0;
/* Field: Changed */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'tmgmt_job_item';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['ui_name'] = 'Changed';
$handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['changed']['alter']['external'] = 0;
$handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['changed']['alter']['html'] = 0;
$handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
$handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
$handler->display->display_options['fields']['changed']['hide_empty'] = 0;
$handler->display->display_options['fields']['changed']['empty_zero'] = 0;
$handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['changed']['date_format'] = 'short';
/* Field: Operations */
$handler->display->display_options['fields']['operations']['id'] = 'operations';
$handler->display->display_options['fields']['operations']['table'] = 'tmgmt_job_item';
$handler->display->display_options['fields']['operations']['field'] = 'operations';
$handler->display->display_options['fields']['operations']['ui_name'] = 'Operations';
$handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['operations']['alter']['external'] = 0;
$handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['operations']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
$handler->display->display_options['fields']['operations']['alter']['html'] = 0;
$handler->display->display_options['fields']['operations']['element_label_colon'] = 0;
$handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
$handler->display->display_options['fields']['operations']['hide_empty'] = 0;
$handler->display->display_options['fields']['operations']['empty_zero'] = 0;
$handler->display->display_options['fields']['operations']['hide_alter_empty'] = 1;
/* Contextual filter: Job Item */
$handler->display->display_options['arguments']['tjid']['id'] = 'tjid';
$handler->display->display_options['arguments']['tjid']['table'] = 'tmgmt_job_item';
$handler->display->display_options['arguments']['tjid']['field'] = 'tjid';
$handler->display->display_options['arguments']['tjid']['ui_name'] = 'Job Item';
$handler->display->display_options['arguments']['tjid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['tjid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['tjid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['tjid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['tjid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['tjid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['tjid']['not'] = 0;
$translatables['tmgmt_ui_job_items'] = array(
  t('Master'),
  t('Job Items'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Label'),
  t('Plugin'),
  t('State'),
  t('Changed'),
  t('Operations'),
  t('All'),
);
