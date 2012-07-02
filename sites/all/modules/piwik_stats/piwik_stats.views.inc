<?php
/**
 * @file
 * Handles views integration of piwik stats field.
 *
 * TODO This implementation is pretty dirty, there should be better ways.
 */

/**
 * Implements hook_field_views_data().
 */
function piwik_stats_field_views_data($field) {
  // Get the default views integration for our field.
  $data = field_views_field_default_views_data($field);

  // Override and extend the default integration.
  foreach ($data as $data_table => &$data_fields) {
    foreach ($data_fields as $data_field_fullname => &$data_field) {
      $data_field_name = str_replace($field['field_name'] . '_', '', $data_field_fullname);

      // Get piwik keyed descriptions.
      $piwik_descriptions = piwik_stats_keyed_descriptions();

      switch ($data_field_name) {
        // Simple integer value fields.
        case 'nb_visits':
        case 'nb_hits':
        case 'entry_nb_visits':
        case 'entry_nb_actions':
        case 'entry_bounce_count':
        case 'exit_nb_visits':
          // Modify title fields.
          $data_field['title'] = str_replace($field['field_name'] . ':' . $data_field_name, $piwik_descriptions[$data_field_name], $data_field['title']);
          $data_field['title short'] = str_replace($data_field_name, $piwik_descriptions[$data_field_name], $data_field['title']);
          // Add field information, so views will regard this as usable field.
          $data_field['field'] = array(
            'table' => $data_table,
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'field_name' => $data_field_fullname,
            'real field' => $data_field_fullname,
            'is revision' => FALSE,
            'entity_tables' => array(
              'node' => 'node',
              'node_revisions' => 'node',
            ),
          );
          break;

        // Date value fields.
        case 'entry_sum_visit_length':
        case 'sum_time_spent':
          // Modify title fields.
          $data_field['title'] = str_replace($field['field_name'] . ':' . $data_field_name, $piwik_descriptions[$data_field_name], $data_field['title']);
          $data_field['title short'] = str_replace($data_field_name, $piwik_descriptions[$data_field_name], $data_field['title']);
          // Add field information, so views will regard this as usable field.
          $data_field['field'] = array(
            'table' => $data_table,
            'handler' => 'views_handler_field_time_interval',
            'click sortable' => TRUE,
            'field_name' => $data_field_fullname,
            'real field' => $data_field_fullname,
            'is revision' => FALSE,
            'entity_tables' => array(
              'node' => 'node',
              'node_revisions' => 'node',
            ),
          );
          break;

        // Percent value fields.
        case 'bounce_rate':
        case 'exit_rate':
          // Modify title fields.
          $data_field['title'] = str_replace($field['field_name'] . ':' . $data_field_name, $piwik_descriptions[$data_field_name] . ' %', $data_field['title']);
          $data_field['title short'] = str_replace($data_field_name, $piwik_descriptions[$data_field_name], $data_field['title']);
          // Add field information, so views will regard this as usable field.
          $data_field['field'] = array(
            'table' => $data_table,
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
            'field_name' => $data_field_fullname,
            'real field' => $data_field_fullname,
            'is revision' => FALSE,
            'entity_tables' => array(
              'node' => 'node',
              'node_revisions' => 'node',
            ),
          );
          break;
      }
    }
  }
  return $data;
}
