<?php

/**
 * Field handler which shows the progressbar.
 *
 * @ingroup views_field_handlers
 */
class tmgmt_handler_field_tmgmt_progress extends views_handler_field_entity {

  /**
   * Prefetch statistics for all jobs.
   */
  function pre_render(&$values) {
    parent::pre_render($values);

    // In case of jobs, pre-fetch the statistics in a single query and add them
    // to the static cache.
    if ($this->entity_type == 'tmgmt_job') {
      $tjids = array();
      foreach ($values as $value) {
        $tjids[] = $value->tjid;
      }
      tmgmt_job_statistics_load($tjids);
    }
  }

  /**
   * Overrides views_handler_field_entity::render().
   */
  function render($values) {
    $object = $this->get_value($values);
    $counts = array(
      '@approved' => $object->getCountApproved(),
      '@translated' => $object->getCountTranslated(),
      '@pending' => $object->getCountPending(),
    );
    $id = $object->internalIdentifier();

    if (module_exists('google_chart_tools')) {
      draw_chart($this->build_progressbar_settings($id, $counts));
      return '<div id="progress' . $id . '"></div>';
    }
    $title = t('Approved: @approved, translated: @translated, pending: @pending.', $counts);
    return sprintf('<span title="%s">%s</span>', $title, implode('/', $counts));
  }

  /**
   * Creates a settings array for the google chart tools.
   *
   * The settings are preset with values to display a progress bar for either
   * a job or job item.
   *
   * @param $id
   *   The id of the chart.
   * @param $counts
   *   Array with the counts for approved, translated and pending.
   * @param $prefix
   *   Prefix to id.
   * @return
   *   Settings array.
   */
  function build_progressbar_settings($id, $counts, $prefix = 'progress') {
    $settings['chart'][$prefix . $id] = array(
      'header' => array(t('Progress')),
      'rows' => array(
        array($counts['@approved']),
        array($counts['@translated']),
        array($counts['@pending']),
      ),
      'columns' => array(t('Approved'), t('Translated'), t('Pending')),
      'chartType' => 'BarChart',
      'containerId' => $prefix . $id,
      'options' => array(
        'forceIFrame' => FALSE,
        'hAxis' => array(
          'textPosition' => 'none',
          'viewWindowMode' => 'maximized',
        ),
        'vAxis' => array(
          'textPosition' => 'none',
          'gridlines' => array('count' => 0),
        ),
        'chartArea' => array(
          'left' => 0,
          'top' => 0,
          'width' => '100%',
          'height' => '100%',
        ),
        'fontSize' => 9,
        'title' => t('Progress'),
        'titlePosition' => 'none',
        'width' => 80,
        'height' => 40,
        'isStacked' => TRUE,
        'legend' => array('position' => 'none'),
      )
    );
    return $settings;
  }

}
