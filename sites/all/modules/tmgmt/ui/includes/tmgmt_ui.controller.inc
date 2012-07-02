<?php

/**
 * @file
 * Please supply a file description.
 */

/**
 * Entity UI controller for the Translator Entity.
 */
class TMGMTTranslatorUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    // We don't need the entire entity label here.
    $items[$this->path]['title'] = 'Translators';
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    $items[$this->path . '/add']['title'] = 'Add Translator';
    unset($items[$this->path . '/add']['title callback']);
    unset($items[$this->path . '/add']['title arguments']);
    if (!empty($this->entityInfo['exportable'])) {
      $items[$this->path . '/import']['title'] = 'Import Translator';
      unset($items[$this->path . '/import']['title callback']);
      unset($items[$this->path . '/import']['title arguments']);
    }
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewForm().
   */
  public function overviewForm($form, &$form_state) {
    $form['translators']['#tree'] = TRUE;
    $form['translators']['#theme'] = 'tmgmt_ui_translator_overview_form';
    $form['translators']['#entity_info'] = $this->entityInfo;
    // Load all translator entities.
    $translators = tmgmt_translator_load_multiple(FALSE);
    foreach ($translators as $key => $translator) {
      $form['translators'][$key] = $this->overviewFormRow(array(), $form_state, $translator, $key);
      $form['translators'][$key]['#translator'] = $translator;
    }
    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewFormSubmit().
   */
  public function overviewFormSubmit($form, &$form_state) {
    // Update image effect weights.
    if (!empty($form_state['values']['translators'])) {
      $translators = tmgmt_translator_load_multiple(array_keys($form_state['values']['translators']));
      foreach ($form_state['values']['translators'] as $key => $item) {
        if (isset($translators[$key])) {
          $translators[$key]->weight = $item['weight'];
          entity_save($this->entityType, $translators[$key]);
        }
      }
    }
  }

  /**
   * Helper method for building a row in the overview form.
   */
  protected function overviewFormRow($form, &$form_state, $entity, $id) {
    $form['#weight'] = isset($form_state['input']['translators']) ? $form_state['input']['translators'][$id]['weight'] : NULL;
    $form['label'] = array(
      '#theme' => 'tmgmt_ui_translator_overview_item',
      '#attached' => array('css' => array(drupal_get_path('module', 'tmgmt_ui') . '/css/tmgmt_ui.admin.css')),
      '#label' => $entity->label(),
      '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
      '#url' => FALSE,
      '#description' => $entity->description,
      '#entity_type' => $this->entityType,
    );
    // Add a row for the exportable status.
    if (!empty($this->entityInfo['exportable'])) {
      $form['status'] = array(
        '#theme' => 'entity_status',
        '#status' => $entity->{$this->statusKey},
      );
    }
    $wrapper = entity_metadata_wrapper($this->entityType, $entity);
    // Add a column to show the translator plugin via the metadata wrapper.
    $form['plugin'] = array(
      '#markup' => $wrapper->plugin->label(),
      '#status' => $entity->{$this->statusKey},
    );
    $controller = $entity->getController();
    $form['configured'] = array(
      '#markup' => $controller->isAvailable($entity) ? t('Yes') : t('No'),
    );
    $form['weight'] = array(
      '#type' => 'weight',
      '#delta' => 30,
      '#default_value' => $entity->weight,
    );
    // Add operations depending on the status.
    if (entity_has_status($this->entityType, $entity, ENTITY_FIXED)) {
      $form['operations']['clone'] = array(
        '#type' => 'link',
        '#title' => t('clone'),
        '#href' => $this->path . '/manage/' . $id . '/clone',
      );
    }
    else {
      $form['operations']['edit'] = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => $this->path . '/manage/' . $id,
      );
      if (!empty($this->entityInfo['exportable'])) {
        $form['operations']['clone'] = array(
          '#type' => 'link',
          '#title' => t('clone'),
          '#href' => $this->path . '/manage/' . $id . '/clone',
        );
      }
      if (empty($this->entityInfo['exportable']) || !entity_has_status($this->entityType, $entity, ENTITY_IN_CODE)) {
        $form['operations']['delete'] = array(
          '#type' => 'link',
          '#title' => t('delete'),
          '#href' => $this->path . '/manage/' . $id . '/delete',
          '#options' => array('query' => drupal_get_destination()),
        );
      }
      elseif (entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
        $form['operations']['revert'] = array(
          '#type' => 'link',
          '#title' => t('revert'),
          '#href' => $this->path . '/manage/' . $id . '/revert',
          '#options' => array('query' => drupal_get_destination()),
        );
      }
      else {
        $row[] = '';
      }
    }
    if (!empty($this->entityInfo['exportable'])) {
      $form['operations']['export'] = array(
        '#type' => 'link',
        '#title' => t('export'),
        '#href' => $this->path . '/manage/' . $id . '/export',
      );
    }
    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::applyOperation().
   */
  public function applyOperation($op, $entity) {
    if ($op == 'delete' && tmgmt_translator_busy($entity->name)) {
      drupal_set_message(t("The translator %translator could not be deleted because it is currently being used by at least one active translation job.", array('%translator' => $entity->label())), 'error');
      return FALSE;
    }
    return parent::applyOperation($op, $entity);
  }

}


/**
 * Entity UI controller for the Job Entity.
 */
class TMGMTJobUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $items[$this->path . '/' . $wildcard] = array(
      'title callback' => 'entity_label',
      'title arguments' => array($this->entityType, $id_count),
      'page callback' => 'tmgmt_ui_job_view',
      'page arguments' => array($id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'tmgmt_job_access',
      'access arguments' => array('view', $id_count),
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => $this->entityInfo['admin ui']['file path'],
    );
    $items[$this->path . '/' . $wildcard . '/view'] = array(
      'title' => 'View',
      'load arguments' => array($this->entityType),
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items[$this->path . '/' . $wildcard . '/manage'] = array(
      'title' => 'Manage',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, $id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'tmgmt_job_access',
      'access arguments' => array('update', $id_count),
      'type' => MENU_LOCAL_TASK,
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => $this->entityInfo['admin ui']['file path'],
    );
    // Menu item for operations like cancel.
    $items[$this->path . '/' . $wildcard . '/%'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_operation_form', $this->entityType, $id_count, $id_count + 1),
      'load arguments' => array($this->entityType),
      'access callback' => 'tmgmt_job_access',
      'access arguments' => array('update', $this->entityType, $id_count),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::operationForm().
   */
  public function operationForm($form, &$form_state, $entity, $op) {
    switch ($op) {
      case 'cancel':
        $confirm_question = t('Are you sure you want to cancel the translation job %label?', array('%label' => $entity->label()));
        return confirm_form($form, $confirm_question, $this->path);
      case 'delete':
        $confirm_question = t('Are you sure you want to delete the translation job %label?', array('%label' => $entity->label()));
        return confirm_form($form, $confirm_question, $this->path);
    }
    drupal_not_found();
    exit;
  }

  /**
   * Overrides EntityDefaultUIController::applyOperation().
   */
  public function applyOperation($op, $entity) {
    switch ($op) {
      case 'delete':
        $entity->delete();
        return t('Deleted the translation job %label.', array('%label' => $entity->label()));
      case 'cancel':
        $entity->cancelTranslation();
        return t('Cancelled the translation job %label.', array('%label' => $entity->label()));
    }
    return FALSE;
  }

}

/**
 * Entity UI controller for the Job Entity.
 */
class TMGMTJobItemUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $items[$this->path . '/' . $wildcard] = array(
      'title callback' => 'entity_label',
      'title arguments' => array($this->entityType, $id_count),
      'page callback' => 'tmgmt_ui_job_item_view',
      'page arguments' => array($id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'tmgmt_job_item_access',
      'access arguments' => array('view', $id_count),
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => $this->entityInfo['admin ui']['file path'],
    );
    $items[$this->path . '/' . $wildcard . '/view'] = array(
      'title' => 'View',
      'load arguments' => array($this->entityType),
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items[$this->path . '/' . $wildcard . '/review'] = array(
      'title' => 'Review',
      'page callback' => 'tmgmt_ui_job_item_review',
      'page arguments' => array($id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'tmgmt_job_item_review_access',
      'access arguments' => array($id_count),
      'type' => MENU_LOCAL_TASK,
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => $this->entityInfo['admin ui']['file path'],
    );
    // Menu item for operations like cancel.
    $items[$this->path . '/' . $wildcard . '/%'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_operation_form', $this->entityType, $id_count, $id_count + 1),
      'load arguments' => array($this->entityType),
      'access callback' => 'tmgmt_job_item_access',
      'access arguments' => array('update', $this->entityType, $id_count),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::operationForm().
   */
  public function operationForm($form, &$form_state, $entity, $op) {
    $controller = $entity->getSourceController();
    $info = $controller->pluginInfo();
    switch ($op) {
      case 'delete':
        $confirm_question = t('Are you sure you want to delete the %plugin translation job item for %label?', array('%plugin' => $info['label'], '%label' => $entity->label()));
        return confirm_form($form, $confirm_question, $this->path);
      case 'accept':
        $confirm_question = t('Are you sure you want to accept the %plugin translation job item for %label?', array('%plugin' => $info['label'], '%label' => $entity->label()));
        return confirm_form($form, $confirm_question, $this->path);
    }
    drupal_not_found();
    exit;
  }

  /**
   * Overrides EntityDefaultUIController::applyOperation().
   */
  public function applyOperation($op, $entity) {
    switch ($op) {
      case 'delete':
        $entity->delete();
        return t('The translation job item %label has been deleted.', array('%label' => $entity->label()));
      case 'accept':
        $entity->accepted('The translation job item has been accepted by !user.', array('!user' => theme('username', array('account' => $GLOBALS['user']))));
        return t('The translation job item %label has been accepted.', array('%label' => $entity->label()));
    }
    return FALSE;
  }

}

/**
 * Adds a description to the translator entity on the entity overview form.
 *
 * @see theme_entity_ui_overview_item()
 */
function theme_tmgmt_ui_translator_overview_item($variables) {
  $output = theme('entity_ui_overview_item', $variables);
  if (!empty($variables['description'])) {
    $output = '<div class="tmgmt-ui-translator-label-wrapper">' . $output . '<div class="description">' . $variables['description'] . '</div></div>';
  }
  return $output;
}

/**
 * Theme callback for adding the tabledrag to the translator entity overview
 * form.
 */
function theme_tmgmt_ui_translator_overview_form($variables) {
  $form = $variables['form'];
  $colspan = !empty($form['#entity_info']['exportable']) ? 4 : 3;
  $rows = array();
  $header = array(
    t('Label'),
    t('Plugin'),
    t('Configured'),
    t('Status'),
    array('data' => t('Operations'), 'colspan' => $colspan),
    t('Weight'),
  );
  foreach (element_children($form) as $key) {
    $row = array();
    $form[$key]['weight']['#attributes']['class'] = array('tmgmt-ui-translator-weight');
    $row[] = drupal_render($form[$key]['label']);
    $row[] = drupal_render($form[$key]['plugin']);
    $row[] = drupal_render($form[$key]['configured']);
    $row[] = drupal_render($form[$key]['status']);
    $operations = element_children($form[$key]['operations']);
    foreach ($operations as $op) {
      $row[] = array('data' => $form[$key]['operations'][$op]);
    }
    $row[] = drupal_render($form[$key]['weight']);
    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }
  drupal_add_tabledrag('tmgmt-ui-translator-overview', 'order', 'sibling', 'tmgmt-ui-translator-weight');
  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('None.'), 'attributes' => array('id' => 'tmgmt-ui-translator-overview')));
}
