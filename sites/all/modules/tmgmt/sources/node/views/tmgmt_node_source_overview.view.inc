<?php

$view = new view;
$view->name = 'tmgmt_node_source_overview';
$view->description = 'Node source overview for bulk operations.';
$view->tag = 'Translation Management';
$view->base_table = 'node';
$view->human_name = 'Node Source Overview';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Content overview';
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer tmgmt';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
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
/* Relationship: User */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['ui_name'] = 'User';
$handler->display->display_options['relationships']['uid']['label'] = 'User';
$handler->display->display_options['relationships']['uid']['required'] = 0;
/* Field: Bulk operations */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['ui_name'] = 'Bulk operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '<!--views-bulk-operations-select-all-->';
$handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
  'action::node_assign_owner_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_message_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_script_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_make_sticky_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_make_unsticky_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_modify_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'show_all_tokens' => 1,
      'display_values' => array(
        '_all_' => '_all_',
      ),
    ),
  ),
  'action::views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'url' => '',
    ),
  ),
  'action::node_promote_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_publish_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_goto_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpromote_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_save_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_send_email_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpublish_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::node_unpublish_by_keyword_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
/* Field: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['ui_name'] = 'Title';
$handler->display->display_options['fields']['title']['label'] = 'Title (in source language)';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['ui_name'] = 'Type';
$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['type']['alter']['external'] = 0;
$handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['type']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['type']['alter']['html'] = 0;
$handler->display->display_options['fields']['type']['element_label_colon'] = 1;
$handler->display->display_options['fields']['type']['element_default_classes'] = 1;
$handler->display->display_options['fields']['type']['hide_empty'] = 0;
$handler->display->display_options['fields']['type']['empty_zero'] = 0;
$handler->display->display_options['fields']['type']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['type']['link_to_node'] = 0;
$handler->display->display_options['fields']['type']['machine_name'] = 0;
/* Field: Source */
$handler->display->display_options['fields']['language']['id'] = 'language';
$handler->display->display_options['fields']['language']['table'] = 'node';
$handler->display->display_options['fields']['language']['field'] = 'language';
$handler->display->display_options['fields']['language']['ui_name'] = 'Source';
$handler->display->display_options['fields']['language']['label'] = 'Source';
$handler->display->display_options['fields']['language']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['language']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['language']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['language']['alter']['external'] = 0;
$handler->display->display_options['fields']['language']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['language']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['language']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['language']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['language']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['language']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['language']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['language']['alter']['trim'] = 0;
$handler->display->display_options['fields']['language']['alter']['html'] = 0;
$handler->display->display_options['fields']['language']['element_label_colon'] = 1;
$handler->display->display_options['fields']['language']['element_default_classes'] = 1;
$handler->display->display_options['fields']['language']['hide_empty'] = 0;
$handler->display->display_options['fields']['language']['empty_zero'] = 0;
$handler->display->display_options['fields']['language']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['language']['link_to_node'] = 0;
$handler->display->display_options['fields']['language']['native_language'] = 0;
/* Field: All translation languages */
$handler->display->display_options['fields']['translation_language_status_1']['id'] = 'translation_language_status_1';
$handler->display->display_options['fields']['translation_language_status_1']['table'] = 'node';
$handler->display->display_options['fields']['translation_language_status_1']['field'] = 'translation_language_status';
$handler->display->display_options['fields']['translation_language_status_1']['ui_name'] = 'All translation languages';
$handler->display->display_options['fields']['translation_language_status_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['translation_language_status_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['translation_language_status_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['translation_language_status_1']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['translation_language_status_1']['not'] = 0;
/* Field: Author */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['ui_name'] = 'Author';
$handler->display->display_options['fields']['name']['label'] = 'Author';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['external'] = 0;
$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
$handler->display->display_options['fields']['name']['format_username'] = 1;
/* Field: Updated date */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'node';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['ui_name'] = 'Updated date';
$handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['changed']['alter']['external'] = 0;
$handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['changed']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['changed']['alter']['html'] = 0;
$handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
$handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
$handler->display->display_options['fields']['changed']['hide_empty'] = 0;
$handler->display->display_options['fields']['changed']['empty_zero'] = 0;
$handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['changed']['date_format'] = 'short';
/* Sort criterion: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['ui_name'] = 'Post date';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter criterion: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['ui_name'] = 'Published';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['exposed'] = TRUE;
$handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
$handler->display->display_options['filters']['status']['expose']['use_operator'] = FALSE;
$handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
$handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
$handler->display->display_options['filters']['status']['expose']['required'] = 1;
$handler->display->display_options['filters']['status']['expose']['multiple'] = FALSE;
/* Filter criterion: Translatable types */
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['id'] = 'tmgmt_translatable_types_1';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['table'] = 'node';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['field'] = 'tmgmt_translatable_types';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['ui_name'] = 'Translatable types';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['group'] = 1;
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['expose']['label'] = 'Translatable types';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['expose']['operator'] = 'tmgmt_translatable_types_1_op';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['expose']['identifier'] = 'types';
$handler->display->display_options['filters']['tmgmt_translatable_types_1']['expose']['reduce'] = 0;
/* Filter criterion: Source translation */
$handler->display->display_options['filters']['source_translation']['id'] = 'source_translation';
$handler->display->display_options['filters']['source_translation']['table'] = 'node';
$handler->display->display_options['filters']['source_translation']['field'] = 'source_translation';
$handler->display->display_options['filters']['source_translation']['ui_name'] = 'Source translation';
$handler->display->display_options['filters']['source_translation']['operator'] = '1';
$handler->display->display_options['filters']['source_translation']['group'] = 1;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/config/regional/tmgmt/node';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Node source';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$translatables['tmgmt_node_source_overview'] = array(
  t('Master'),
  t('Content overview'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('User'),
  t('<!--views-bulk-operations-select-all-->'),
  t('Title (in source language)'),
  t('Type'),
  t('Source'),
  t('All translation languages'),
  t('Author'),
  t('Updated date'),
  t('Published'),
  t('Translatable types'),
  t('Page'),
);
