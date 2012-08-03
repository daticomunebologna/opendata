<?php

/**
 * Field handler which shows the operations for a job item.
 *
 * @todo Remove this once http://drupal.org/node/1435662 is through.
 *
 * @ingroup views_field_handlers
 */
class tmgmt_handler_field_tmgmt_job_item_operations extends views_handler_field_entity {

  function render($values) {
    $item = $this->get_value($values);
    $element = array();
    $element['#theme'] = 'links';
    $element['#attributes'] = array('class' => array('inline'));
    $uri = $item->uri();
    $element['#links']['view'] = array(
      'href' => $uri['path'],
      'title' => t('view'),
    );
    if ($item->isNeedsReview() && entity_access('accept', 'tmgmt_job_item', $item)) {
      $element['#links']['review'] = array(
        'href' => 'admin/config/regional/tmgmt/items/' . $item->tjiid . '/review',
        'query' => array('destination' => current_path()),
        'title' => t('review'),
      );
    }
    if (user_access('administer tmgmt')) {
      $element['#links']['delete'] = array(
        'href' => 'admin/config/regional/tmgmt/items/' . $item->tjiid . '/delete',
        'query' => array('destination' => current_path()),
        'title' => t('delete'),
      );
    }
    return drupal_render($element);
  }

}
