<?php

/**
 * @file
 * Theme file stub for tmgmt.
 */

/**
 * Generic theme function for use with Render API.
 *
 * Renders the #title and #attributes properties if they are present.
 */
function theme_tmgmt_ui_element($variables) {
  $element = $variables['element'];

  // Use the #title attribute.
  $title = '';
  if (!empty($element['#title'])) {
    $title = '<h3>' . check_plain($element['#title']) . '</h3>';
  }

  // Use #attributes to customize a wrapper <div>.
  $attributes = '';
  if (!empty($element['#attributes'])) {
    $attributes = drupal_attributes($element['#attributes']);
  }

  // Render any child items.
  if (!$element['#children']) {
    $element['#children'] = drupal_render_children($element);
  }

  // Build simple output.
  $output = "<div{$attributes}>{$title}{$element['#children']}</div>";
  return $output;
}

/**
 * Gets translation language status.
 *
 * @param array $variables
 *   - 'translation_status': A flag that determines the status. Possible values:
 *     original, missing, outofdate.
 *   - 'job_item': Current job item entity associated with translation.
 *
 * @return string
 *   Icon or a link icon that explains the translation status and possibly
 *   links to an active translation job.
 */
function theme_tmgmt_ui_translation_language_status_single($variables) {

  switch ($variables['translation_status']) {
    case 'original':
      $label = t('Source');
      break;

    case 'missing':
      $label = t('Missing');
      break;

    case 'outofdate':
      $label = t('Outdated');
      break;

    default:
      $label = t('Current');
  }

  // Override the image in case of an active job.
  if (isset($variables['job_item'])) {
    $wrapper = entity_metadata_wrapper('tmgmt_job_item', $variables['job_item']);

    $label = t('Active job item: @state', array('@state' => $wrapper->state->label()));

    switch ($variables['job_item']->state) {
      case TMGMT_JOB_ITEM_STATE_ACTIVE:
        $variables['translation_status'] = 'active';
        break;

      case TMGMT_JOB_ITEM_STATE_REVIEW:
        $variables['translation_status'] = 'translated';
        break;
    }
  }

  $url = drupal_get_path('module', 'tmgmt_ui') . '/images/' . $variables['translation_status'] . '.png';

  // Theme the image.
  $image_variables = array(
    'path' => $url,
    'alt' => $label,
    'title' => $label,
    'attributes' => array(
      'class' => array('tmgmt-translation-status-image', 'tmgmt-translation-status-' . $variables['translation_status']),
    ),
  );
  $status = theme('image', $image_variables);

  // If we have an active job item, wrap it in a link.
  if (!empty($variables['job_item'])) {
    $uri = $variables['job_item']->uri();
    // If the job item needs to be reviewed, point to the review form.
    // Otherwise just use the plain entity uri.
    $path = $variables['job_item']->isNeedsReview() ? $uri['path'] . '/review' : $uri['path'];
    $status = l($status, $path, array(
      'query' => array('destination' => current_path()),
      'html' => TRUE,
      'attributes' => array('title' => $label),
    ));
  }
  return $status;
}

/**
 * Renders a data item status as an HTML/CSS icon.
 */
function theme_tmgmt_ui_translator_review_form_element_status($variables) {
  $classes = array();
  $classes[] = 'tmgmt-ui-icon';
  $classes[] = 'tmgmt-ui-icon-32'; // Icon size 32px square.
  switch ($variables['status']['#value']) {
    case TMGMT_DATA_ITEM_STATE_APPROVED:
      $title = t('Approved');
      // Unicode character U+2611 BALLOT BOX WITH CHECK.
      $icon = '&#x2611;';
      $classes[] = 'tmgmt-ui-icon-green';
      $classes[] = 'tmgmt-ui-state-approved';
      break;
    case TMGMT_DATA_ITEM_STATE_TRANSLATED:
      $title = t('Translated');
      // Unicode character U+2610 BALLOT BOX.
      $icon = '&#x2610;';
      $classes[] = 'tmgmt-ui-icon-yellow';
      $classes[] = 'tmgmt-ui-state-translated';
      break;
    case TMGMT_DATA_ITEM_STATE_PENDING:
    default:
      $title = t('Pending');
      // Just an empty icon without a sign.
      $icon = '';
      $classes[] = 'tmgmt-ui-icon-grey';
      $classes[] = 'tmgmt-ui-state-pending';
      break;
  }
  return sprintf('<div class="%s" title="%s"><span>%s</span></div>', implode(' ', $classes), $title, $icon);
}

/**
 * Render one single data item as a table row.
 */
function theme_tmgmt_ui_translator_review_form_element($variables) {
  $element = $variables['element'];
  // Label of all element groups.
  if (!isset($element['#top_label'])) {
    $element['#top_label'] = array_shift($element['#parent_label']);
  }
  // Label of the current data item.
  if (!isset($element['#leave_label'])) {
    $element['#leave_label'] = array_pop($element['#parent_label']);
  }
  // Do not repeat labels inside the same hierarchy.
  if ($element['#top_label'] == $element['#leave_label']) {
    $element['#leave_label'] = '';
  }
  $result = '<tr' . drupal_attributes(array('class' => $element['#zebra'], 'id' => $element['#ajaxid'])) . '>';
  $row = array();
  $leaf_label = '<label>' . $element['#leave_label'] . '</label>';
  $leaf_label = '<div class="form-item form-type-label">' . $leaf_label . '</div>';
  $row[] = array(
    'data' => $leaf_label . '<div class="tmgmt-ui-state">' . drupal_render($element['status']) . '</div>',
    'class' => 'tmgmt-ui-data-item-label',
  );
  $row[] = array(
    'data' => drupal_render($element['source']),
    'class' => 'tmgmt-ui-data-item-source',
  );
  $row[] = array(
    'data' => drupal_render($element['translation']),
    'class' => 'tmgmt-ui-data-item-translation',
  );
  $row[] = array(
    'data' => drupal_render($element['actions']),
    'class' => 'tmgmt-ui-data-item-label',
  );
  foreach ($row as $cell) {
    $result .= _theme_table_cell($cell);
  }
  $result .= '</tr>';
  return $result;
}

/**
 * Renders a table containing a group of data items belonging to the same field.
 */
function theme_tmgmt_ui_translator_review_form($variables) {
  $element = $variables['element'];
  $result = '';
  $labels = '';
  $parent_label = '';
  $element_groups = array();
  $element_group = '';
  foreach (element_children($element) as $key) {
    // Label of all element groups.
    $parent_label = array_shift($element[$key]['#parent_label']);
    $element[$key]['#top_label'] = $parent_label;
    $element[$key]['#leave_label'] = array_pop($element[$key]['#parent_label']);
    // Start a new element group.
    if ($labels != $element[$key]['#parent_label']) {
      $labels = $element[$key]['#parent_label'];
      if (!empty($labels)) {
        // Append to previous group to the group collection.
        if (!empty($element_group)) {
          $element_groups[] = '<tbody>' . $element_group . '</tbody>';
        }
        // Header row for the current element group.
        $cell = array(
          // @todo: Deal with right to left languages.
          'data' => check_plain(implode(t(' > '), $labels)),
          'colspan' => 4,
        );
        $element_group = '<tr>' . _theme_table_cell($cell, TRUE) . '</tr>';
      }
    }
    $element_group .= drupal_render($element[$key]);
  }

  // Append the last group to the group collection.
  $element_groups[] = '<tbody>' . $element_group . '</tbody>';

  // Display the label of all element groups inside a table header.
  if (!empty($parent_label)) {
    $cell = array(
      'data' => $parent_label,
      'colspan' => 5,
    );
    $result = '<thead><tr>' . _theme_table_cell($cell, TRUE) . '</tr></thead>' . implode('', $element_groups);
  }
  return '<div id="tmgmt-status-messages-' . strtolower($parent_label) . '"></div><table class="tmgmt-ui-review"><colgroup width="100" /><colgroup width="*" span="2" /><colgroup width="100" />' . $result . '</table>';
}
