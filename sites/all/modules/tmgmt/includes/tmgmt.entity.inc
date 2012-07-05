<?php

/*
 * @file
 * Entity classes for Translation Management entities.
 */

/**
 * Entity class for the tmgmt_translator entity.
 *
 * @ingroup tmgmt_translator
 */
class TMGMTTranslator extends Entity {

  /**
   * The ID of the translator.
   *
   * @var integer
   */
  public $tid;

  /**
   * Machine readable name of the translator.
   *
   * @var string
   */
  public $name;

  /**
   * Label of the translator.
   *
   * @var string
   */
  public $label;

  /**
   * Description of the translator.
   *
   * @var string
   */
  public $description;

  /**
   * Weight of the translator.
   *
   * @var int
   */
  public $weight;

  /**
   * Plugin name of the translator.
   *
   * @type string
   */
  public $plugin;

  /**
   * Translator type specific settings.
   *
   * @var array
   */
  public $settings;

  /**
   * The supported target languages caches.
   *
   * @var array
   */
  protected $languageCache;

  /**
   * Whether the language cache in the database is outdated.
   *
   * @var boolean
   */
  protected $languageCacheOutdated;

  /**
   * Overrides Entity::__construct().
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'tmgmt_translator');
  }

  /**
   * Returns the translator plugin controller of this translator.
   *
   * @return TMGMTTranslatorPluginControllerInterface
   */
  public function getController() {
    if (!empty($this->plugin)) {
      return tmgmt_translator_plugin_controller($this->plugin);
    }
    return FALSE;
  }

  /**
   * Returns the supported target languages for this translator.
   *
   * @return array
   *   An array of supported target languages in ISO format.
   */
  public function getSupportedTargetLanguages($source_language) {
    if ($controller = $this->getController()) {
      if (isset($this->pluginInfo['cache languages']) && empty($this->pluginInfo['cache languages'])) {
        // This plugin doesn't support language caching.
        return $controller->getSupportedTargetLanguages($this, $source_language);
      }
      else {
        // Retrieve the supported languages from the cache.
        if (empty($this->languageCache) && $cache = cache_get('languages:' . $this->name, 'cache_tmgmt')) {
          $this->languageCache = $cache->data;
        }
        // Even if we successfully queried the cache it might not have an entry
        // for our source language yet.
        if (!isset($this->languageCache[$source_language])) {
          $this->languageCache[$source_language] = $controller->getSupportedTargetLanguages($this, $source_language);
          $this->languageCacheOutdated = TRUE;
        }
      }
      return $this->languageCache[$source_language];
    }
  }

  /**
   * Check whether this translator can handle a particular translation job.
   *
   * @param $job
   *   The TMGMTJob entity that should be translated.
   *
   * @return boolean
   *   TRUE if the job can be processed and translated, FALSE otherwise.
   */
  public function canTranslate(TMGMTJob $job) {
    if ($controller = $this->getController()) {
      return $controller->canTranslate($this, $job);
    }
    return FALSE;
  }

  /**
   * Checks whether a translator is available.
   *
   * @return boolean
   *   TRUE if the translator plugin is available, FALSE otherwise.
   */
  public function isAvailable() {
    if ($controller = $this->getController()) {
      return $controller->isAvailable($this);
    }
    return FALSE;
  }

  /**
   * Returns if the plugin has any settings for this job.
   */
  public function hasCheckoutSettings(TMGMTJob $job) {
    if ($controller = $this->getController()) {
      return $controller->hasCheckoutSettings($job);
    }
    return FALSE;
  }

  /**
   * @todo Remove this once http://drupal.org/node/1420364 is done.
   */
  public function getNotAvailableReason() {
    if ($controller = $this->getController()) {
      return $controller->getNotAvailableReason($this);
    }
    return FALSE;
  }

  /**
   * Retrieves a setting value from the translator settings. Pulls the default
   * values (if defined) from the plugin controller.
   *
   * @param $name
   *   The name of the setting.
   *
   * @return
   *   The setting value or $default if the setting value is not set. Returns
   *   NULL if the setting does not exist at all.
   */
  public function getSetting($name) {
    if (isset($this->settings[$name])) {
      return $this->settings[$name];
    }
    elseif ($controller = $this->getController()) {
      $defaults = $controller->defaultSettings();
      if (isset($defaults[$name])) {
        return $defaults[$name];
      }
    }
  }

  /**
   * Updates the language cache if it has changed.
   */
  public function __destruct() {
    if ($controller = $this->getController()) {
      $info = $controller->pluginInfo();
      if (!isset($info['language cache']) || !empty($info['language cache']) && !empty($this->languageCacheOutdated)) {
        cache_set('languages:' . $this->name, $this->languageCache, 'cache_tmgmt');
      }
    }
  }

}

/**
 * Entity class for the tmgmt_job entity.
 *
 * @ingroup tmgmt_job
 */
class TMGMTJobItem extends Entity implements TMGMTDataProvider {

  /**
   * The source plugin that provides the item.
   *
   * @var varchar
   */
  public $plugin;

  /**
   * The identifier of the translation job.
   *
   * @var integer
   */
  public $tjid;

  /**
   * The identifier of the translation job item.
   *
   * @var integer
   */
  public $tjiid;

  /**
   * Type of this item, used by the plugin to identify it.
   *
   * @var string
   */
  public $item_type;

  /**
   * Id of the item.
   *
   * @var integer
   */
  public $item_id;

  /**
   * The time when the job item was changed as a timestamp.
   *
   * @var integer
   */
  public $changed;

  /**
   * Can be used by the source plugin to store the data instead of creating it
   * on demand.
   *
   * If additional information is added in the UI, like adding comments, it will
   * also be saved here.
   *
   * Always use TMGMTJobItem::getSourceData() to load the data, which will use
   * this property if present and otherwise get it from the source.
   *
   * @var array
   */
  public $data = array();

  /**
   * The translated data in the same structure.
   *
   * Only the parts that actually are translated are present in this array.
   *
   * @var array
   */
  public $translation = array();

  /**
   * Overrides Entity::__construct().
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'tmgmt_job_item');
    if (!isset($this->state)) {
      $this->state = TMGMT_JOB_ITEM_STATE_ACTIVE;
    }
  }

  /**
   * Overrides Entity::defaultLabel()
   */
  public function defaultLabel() {
    if ($controller = $this->getSourceController()) {
      return t('Translation for @label', array('@label' => $controller->getLabel($this)));
    }
    return parent::defaultLabel();
  }

  /**
   * Overrides Entity::defaultUri()
   *
   * @see _tmgmt_ui_breadcrumb()
   */
  public function defaultUri() {
    // The path of a job item is not directly below the job that it belongs to.
    // Having to maintain two unknowns / wildcards (job and job item) in the
    // path is more complex than it has to be. Instead we just append the
    // additional breadcrumb pieces manually with _tmgmt_ui_breadcrumb().
    return array('path' => 'admin/config/regional/tmgmt/items/' . $this->tjiid);
  }

  /**
   * Overrides Entity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $wrapper = entity_metadata_wrapper('tmgmt_job_item', $this);
    $content['changed'] = array(
      '#type' => 'item',
      '#title' => t('Last change'),
      '#markup' => format_date($wrapper->changed->value()),
    );
    $content['state'] = array(
      '#type' => 'item',
      '#title' => t('State'),
      '#markup' => $wrapper->state->label(),
    );
    $job = $this->getJob();
    $uri = $job->uri();
    $content['job'] = array(
      '#type' => 'item',
      '#title' => t('Job'),
      '#markup' => l($job->label(), $uri['path']),
    );
    if (module_exists('views') && $view = views_get_view('tmgmt_ui_job_item_messages')) {
      $content['messages'] = array(
        '#type' => 'item',
        '#title' => $view->get_title(),
        '#markup' => $view->preview('block', array($this->tjiid)),
      );
    }
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Add a log message for this job item.
   *
   * @param $message
   *   The message to store in the log. Keep $message translatable by not
   *   concatenating dynamic values into it! Variables in the message should be
   *   added by using placeholder strings alongside the variables argument to
   *   declare the value of the placeholders. See t() for documentation on how
   *   $message and $variables interact.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   * @param $type
   *   (Optional) The type of the message. Can be one of 'status', 'error',
   *   'warning' or 'debug'. Messages of the type 'debug' will not get printed
   *   to the screen.
   */
  public function addMessage($message, $variables = array(), $type = 'status') {
    // Save the job item if it hasn't yet been saved.
    if (!empty($this->tjiid) || $this->save()) {
      $message = tmgmt_message_create($message, $variables, array('tjid' => $this->tjid, 'tjiid' => $this->tjiid, 'type' => $type));
      if ($message->save()) {
        return $message;
      }
    }
    return FALSE;
  }

  /**
   * Retrieves the label of the source object via the source controller.
   *
   * @return
   *   The label of the source object.
   */
  public function getSourceLabel() {
    if ($controller = $this->getSourceController()) {
      return $controller->getLabel($this);
    }
    return FALSE;
  }

  /**
   * Retrieves the path to the source object via the source controller.
   *
   * @return
   *   The path to the source object.
   */
  public function getSourceUri() {
    if ($controller = $this->getSourceController()) {
      return $controller->getUri($this);
    }
    return FALSE;
  }

  /**
   * Loads the job entity that this job item is attached to.
   *
   * @return TMGMTJob
   *   The job entity that this job item is attached to or FALSE if there was
   *   a problem.
   */
  public function getJob() {
    if (!empty($this->tjid)) {
      return tmgmt_job_load($this->tjid);
    }
    return FALSE;
  }

  /**
   * Returns the translator for this job item.
   *
   * @return TMGMTTranslator
   *   The translator entity or FALSE if there was a problem.
   */
  public function getTranslator() {
    if ($job = $this->getJob()) {
      return $job->getTranslator();
    }
    return FALSE;
  }

  /**
   * Returns the translator plugin controller of the translator of this job item.
   *
   * @return TMGMTTranslatorPluginControllerInterface
   *   The controller of the translator plugin or FALSE if there was a problem.
   */
  public function getTranslatorController() {
    if ($job = $this->getJob()) {
      return $job->getTranslatorController();
    }
    return FALSE;
  }

  /**
   * Array of the data to be translated.
   *
   * The structure is similar to the form API in the way that it is a possibly
   * nested array with the following properties whose presence indicate that the
   * current element is a text that might need to be translated.   *
   * - #text: The text to be translated.
   * - #label: The label that might be shown to the translator.
   * - #comment: (Optional) A comment with additional information.
   * - #translate: Either TRUE or FALSE. Only text that has this property set to
   *   TRUE must be translated.
   *
   * The key can be an alphanumeric string.
   *
   * @param $reset
   *   (Optional) Whether the data should be regenerated from the source object.
   *   Defaults to FALSE.
   *
   * @return array
   *   A data array structured in the
   *
   * @see TMGMTJobItem::getFlattenedData()
   */
  public function getSourceData($reset = FALSE) {
    if ($reset || empty($this->data)) {
      $this->data = $this->getSourceController()->getData($this);
      $this->save();
    }
    return $this->data;
  }

  /**
   * Returns the plugin controller of the configured plugin.
   *
   * @return TMGMTSourcePluginControllerInterface
   */
  public function getSourceController() {
    if (!empty($this->plugin)) {
      return tmgmt_source_plugin_controller($this->plugin);
    }
    return FALSE;
  }

  /**
   * Sets the state of the job item to 'needs review'.
   */
  public function needsReview($message = NULL, $variables = array(), $type = 'status') {
    if (!isset($message)) {
      $uri = $this->getSourceUri();
      $message = 'The translation for !source needs to be reviewed.';
      $variables = array('!source' => l($this->getSourceLabel(), $uri['path']));
    }
    $return = $this->setState(TMGMT_JOB_ITEM_STATE_REVIEW, $message, $variables, $type);
    // Auto accept the translation if the translator is configured for it.
    if (variable_get('tmgmt_auto_accept', FALSE) && $this->getTranslator()->getSetting('auto_accept')) {
      $this->acceptTranslation();
    }
    return $return;
  }

  /**
   * Sets the state of the job item to 'accepted'.
   */
  public function accepted($message = NULL, $variables = array(), $type = 'status') {
    if (!isset($message)) {
      $uri = $this->getSourceUri();
      $message = 'The translation for !source has been accepted.';
      $variables = array('!source' => l($this->getSourceLabel(), $uri['path']));
    }
    $return = $this->setState(TMGMT_JOB_ITEM_STATE_ACCEPTED, $message, $variables, $type);
    // Check if this was the last unfinished job item in this job.
    if (tmgmt_job_check_finished($this->tjid) && $job = $this->getJob()) {
      // Mark the job as finished.
      $job->finished();
    }
    return $return;
  }

  /**
   * Sets the state of the job item to 'active'.
   */
  public function active($message = NULL, $variables = array(), $type = 'status') {
    if (!isset($message)) {
      $uri = $this->getSourceUri();
      $message = 'The translation for !source is now being processed.';
      $variables = array('!source' => l($this->getSourceLabel(), $uri['path']));
    }
    return $this->setState(TMGMT_JOB_ITEM_STATE_ACTIVE, $message, $variables, $type);
  }

  /**
   * Updates the state of the job item.
   *
   * @param $state
   *   The new state of the job item. Has to be one of the job state constants.
   * @param $message
   *   (Optional) The log message to be saved along with the state change.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   *
   * @return int
   *   The updated state of the job if it could be set.
   *
   * @see TMGMTJob::addMessage()
   */
  public function setState($state, $message = NULL, $variables = array(), $type = 'debug') {
    // Return TRUE if the state could be set. Return FALSE otherwise.
    if (array_key_exists($state, tmgmt_job_item_states()) && $this->state != $state) {
      $this->state = $state;
      $this->save();
      // If a message is attached to this state change add it now.
      if (isset($message)) {
        $this->addMessage($message, $variables, $type);
      }
    }
    return $this->state;
  }

  /**
   * Returns the state of the job item. Can be one of the job item state
   * constants.
   *
   * @return integer
   *   The state of the job item.
   */
  public function getState() {
    // We don't need to check if the state is actually set because we always set
    // it in the constructor.
    return $this->state;
  }

  /**
   * Checks whether the passed value matches the current state.
   *
   * @param $state
   *   The value to check the current state against.
   *
   * @return boolean
   *   TRUE if the passed state matches the current state, FALSE otherwise.
   */
  public function isState($state) {
    return $this->getState() == $state;
  }

  /**
   * Checks whether the state of this transaction is 'accepted'.
   *
   * @return boolean
   *   TRUE if the state is 'accepted', FALSE otherwise.
   */
  public function isAccepted() {
    return $this->isState(TMGMT_JOB_ITEM_STATE_ACCEPTED);
  }

  /**
   * Checks whether the state of this transaction is 'active'.
   *
   * @return boolean
   *   TRUE if the state is 'active', FALSE otherwise.
   */
  public function isActive() {
    return $this->isState(TMGMT_JOB_ITEM_STATE_ACTIVE);
  }

  /**
   * Checks whether the state of this transaction is 'needs review'.
   *
   * @return boolean
   *   TRUE if the state is 'needs review', FALSE otherwise.
   */
  public function isNeedsReview() {
    return $this->isState(TMGMT_JOB_ITEM_STATE_REVIEW);
  }

  /**
   * Add translated data to a job item.
   *
   * Note: This does not merge existing data which already exists below the
   * defined key and will replace it.
   *
   * @param $translation
   *   Nested array of translated data. Can either be a single text entry, the
   *   whole data structure or parts of it.
   * @param $key
   *   (Optional) Either a flattened key (a 'key1][key2][key3' string) or a nested
   *   one, e.g. array('key1', 'key2', 'key2'). Defaults to an empty array which
   *   means that it will replace the whole translated data array.
   *
   * @return boolean
   *   TRUE if the translated data could be added, FALSE otherwise.
   */
  public function addTranslatedData($translation, $key = array()) {
    if (empty($key)) {
      $key = array();
    }
    if (!is_array($key)) {
      $key = explode(TMGMT_ARRAY_DELIMITER, $key);
    }
    if (!is_array($this->translation)) {
      $this->translation = array();
    }
    drupal_array_set_nested_value($this->translation, $key, $translation);
    // Check if the job item has all the translated data that it needs now.
    if (!array_diff_key(tmgmt_flatten_data($this->getSourceData()), tmgmt_flatten_data($this->translation))) {
      // There are no unfinished elements left.
      $uri = $this->getSourceUri();
      $this->needsReview('The translation for !source is finished and can now be reviewed.', array('!source' => l($this->getSourceLabel(), $uri['path'])));
    }
    else {
      // We only need to save manually if no no state change was triggered.
      $this->save();
    }
  }

  /**
   * Propagates the returned job item translations to the sources.
   *
   * @return boolean
   *   TRUE if we were able to propagate the translated data and the item could
   *   be saved, FALSE otherwise.
   */
  public function acceptTranslation() {
    if (!$this->isNeedsReview() || !$controller = $this->getSourceController()) {
      return FALSE;
    }
    // We don't know if the source plugin was able to save the translation after
    // this point. That means that the plugin has to set the 'accepted' states
    // on its own.
    $controller->saveTranslation($this);
  }

  /**
   * Retrieves the translated data from a job item.
   *
   * @return
   *   The array of translated data or FALSE if no translated data has been
   *   saved yet.
   */
  public function getTranslatedData() {
    if (!empty($this->translation)) {
      return $this->translation;
    }
    return array();
  }

  /**
   * Returns all job messages attached to this job item.
   *
   * @return array
   *   An array of translation job messages.
   */
  public function getMessages($conditions = array()) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'tmgmt_message');
    $query->propertyCondition('tjiid', $this->tjiid);
    foreach ($conditions as $key => $condition) {
      if (is_array($condition)) {
        $operator = isset($condition['operator']) ? $condition['operator'] : '=';
        $query->propertyCondition($key, $condition['value'], $operator);
      }
      else {
        $query->propertyCondition($key, $condition);
      }
    }
    $results = $query->execute();
    if (!empty($results['tmgmt_message'])) {
      return entity_load('tmgmt_message', array_keys($results['tmgmt_message']));
    }
    return array();
  }

  /**
   * Retrieves all siblings of this job item.
   *
   * @return array
   *   An array of job items that are the siblings of this job item.
   */
  public function getSiblings() {
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'tmgmt_job_item')
      ->propertyCondition('tjiid', $this->tjiid, '<>')
      ->propertyCondition('tjid', $this->tjid)
      ->execute();
    if (!empty($result['tmgmt_job_item'])) {
      return entity_load('tmgmt_job_item', array_keys($result['tmgmt_job_item']));
    }
    return FALSE;
  }

  /**
   * Returns all job messages attached to this job item with timestamp newer
   * than $time.
   *
   * @param $timestamp
   *   (Optional) Messages need to have a newer timestamp than $time. Defaults
   *   to REQUEST_TIME.
   *
   * @return array
   *   An array of translation job messages.
   */
  public function getMessagesSince($time = NULL) {
    $time = isset($time) ? $time : REQUEST_TIME;
    $conditions = array('created' => array('value' => $time, 'operator' => '>='));
    return $this->getMessages($conditions);
  }

}

/**
 * Entity class for the tmgmt_job entity.
 *
 * @ingroup tmgmt_job
 */
class TMGMTJob extends Entity implements TMGMTDataProvider {

  /**
   * Translation job identifier.
   *
   * @var integer
   */
  public $tjid;

  /**
   * A custom label for this job.
   */
  public $label;

  /**
   * Current state of the translation job
   * @var type
   */
  public $state;

  /**
   * Language to be translated from.
   *
   * @var string
   */
  public $source_language;

  /**
   * Language into which the data needs to be translated.
   *
   * @var varchar
   */
  public $target_language;

  /**
   * Reference to the used translator of this job.
   *
   * @see TMGMTJob::getTranslatorController()
   *
   * @var string
   */
  public $translator;

  /**
   * Translator specific configuration and context information for this job.
   *
   * @var array
   */
  public $settings;

  /**
   * Remote identification of this job.
   *
   * @var integer
   */
  public $reference;

  /**
   * The time when the job was created as a timestamp.
   *
   * @var integer
   */
  public $created;

  /**
   * The time when the job was changed as a timestamp.
   *
   * @var integer
   */
  public $changed;

  /**
   * The user id of the creator of the job.
   *
   * @var integer
   */
  public $uid;

  /**
   * Overrides Entity::__construct().
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'tmgmt_job');
    if (empty($this->tjid)) {
      $this->created = REQUEST_TIME;
    }
    if (!isset($this->state)) {
      $this->state = TMGMT_JOB_STATE_UNPROCESSED;
    }
  }

  /**
   * Overrides Entity::defaultLabel().
   */
  public function defaultLabel() {
    // In some cases we might have a user-defined label.
    if (!empty($this->label)) {
      return $this->label;
    }
    $wrapper = entity_metadata_wrapper($this->entityType, $this);
    $source = $wrapper->source_language->label();
    if (empty($source)) {
      $source = '?';
    }
    $target = $wrapper->target_language->label();
    if (empty($target)) {
      $target = '?';
    }
    return t('From @source to @target', array('@source' => $source, '@target' => $target));
  }

  /**
   * Overrides Entity::defaultUri().
   */
  public function defaultUri() {
    return array('path' => 'admin/config/regional/tmgmt/jobs/' . $this->tjid);
  }

  /**
   * Overrides Entity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $wrapper = entity_metadata_wrapper('tmgmt_job', $this);
    $content['created'] = array(
      '#type' => 'item',
      '#title' => t('Created'),
      '#markup' => format_date($wrapper->created->value()),
    );
    $content['changed'] = array(
      '#type' => 'item',
      '#title' => t('Last changed'),
      '#markup' => format_date($wrapper->changed->value()),
    );
    $content['state'] = array(
      '#type' => 'item',
      '#title' => t('State'),
      '#markup' => $wrapper->state->label(),
    );
    if (module_exists('views')) {
      if ($view = views_get_view('tmgmt_ui_job_items')) {
        $content['items'] = array(
          '#type' => 'item',
          '#title' => $view->get_title(),
          '#markup' => $view->preview('block', array($this->tjid)),
        );
      }
      if ($view = views_get_view('tmgmt_ui_job_messages')) {
        $content['messages'] = array(
          '#type' => 'item',
          '#title' => $view->get_title(),
          '#markup' => $view->preview('block', array($this->tjid)),
        );
      }
    }
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Adds an item to the translation job.
   *
   * @param $plugin
   *   The plugin name.
   * @param $item_type
   *   The source item type.
   * @param $item_id
   *   The source item id.
   *
   * @return TMGMTJobItem
   *   The job item that was added to the job or FALSE if it couldn't be saved.
   */
  public function addItem($plugin, $item_type, $item_id) {
    // Save the job if it hasn't yet been saved.
    if (!empty($this->tjid) || $this->save()) {
      $item = tmgmt_job_item_create($plugin, $item_type, $item_id, array('tjid' => $this->tjid));
      if ($item->save()) {
        return $item;
      }
    }
    return FALSE;
  }

  /**
   * Add a log message for this job.
   *
   * @param $message
   *   The message to store in the log. Keep $message translatable by not
   *   concatenating dynamic values into it! Variables in the message should be
   *   added by using placeholder strings alongside the variables argument to
   *   declare the value of the placeholders. See t() for documentation on how
   *   $message and $variables interact.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   * @param $type
   *   (Optional) The type of the message. Can be one of 'status', 'error',
   *   'warning' or 'debug'. Messages of the type 'debug' will not get printed
   *   to the screen.
   */
  public function addMessage($message, $variables = array(), $type = 'status') {
    // Save the job if it hasn't yet been saved.
    if (!empty($this->tjid) || $this->save()) {
      $message = tmgmt_message_create($message, $variables, array('tjid' => $this->tjid, 'type' => $type));
      if ($message->save()) {
        return $message;
      }
    }
    return FALSE;
  }

  /**
   * Returns all job items attached to this job.
   *
   * @return array
   *   An array of translation job items.
   */
  public function getItems($conditions = array()) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'tmgmt_job_item');
    $query->propertyCondition('tjid', $this->tjid);
    foreach ($conditions as $key => $condition) {
      if (is_array($condition)) {
        $operator = isset($condition['operator']) ? $condition['operator'] : '=';
        $query->propertyCondition($key, $condition['value'], $operator);
      }
      else {
        $query->propertyCondition($key, $condition);
      }
    }
    $results = $query->execute();
    if (!empty($results['tmgmt_job_item'])) {
      return entity_load('tmgmt_job_item', array_keys($results['tmgmt_job_item']));
    }
    return array();
  }

  /**
   * Returns all job messages attached to this job.
   *
   * @return array
   *   An array of translation job messages.
   */
  public function getMessages($conditions = array()) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'tmgmt_message');
    $query->propertyCondition('tjid', $this->tjid);
    foreach ($conditions as $key => $condition) {
      if (is_array($condition)) {
        $operator = isset($condition['operator']) ? $condition['operator'] : '=';
        $query->propertyCondition($key, $condition['value'], $operator);
      }
      else {
        $query->propertyCondition($key, $condition);
      }
    }
    $results = $query->execute();
    if (!empty($results['tmgmt_message'])) {
      return entity_load('tmgmt_message', array_keys($results['tmgmt_message']));
    }
    return array();
  }

  /**
   * Returns all job messages attached to this job with timestamp newer than
   * $time.
   *
   * @param $time
   *   (Optional) Messages need to have a newer timestamp than $time. Defaults
   *   to REQUEST_TIME.
   *
   * @return array
   *   An array of translation job messages.
   */
  public function getMessagesSince($time = NULL) {
    $time = isset($time) ? $time : REQUEST_TIME;
    $conditions = array('created' => array('value' => $time, 'operator' => '>='));
    return $this->getMessages($conditions);
  }

  /**
   * Retrieves a setting value from the job settings. Pulls the default values
   * (if defined) from the plugin controller.
   *
   * @param $name
   *   The name of the setting.
   *
   * @return
   *   The setting value or $default if the setting value is not set. Returns
   *   NULL if the setting does not exist at all.
   */
  public function getSetting($name) {
    if (isset($this->settings[$name])) {
      return $this->settings[$name];
    }
    // The translator might provide default settings.
    if ($translator = $this->getTranslator()) {
      if (($setting = $translator->getSetting($name)) !== NULL) {
        return $setting;
      }
    }
    if ($controller = $this->getTranslatorController()) {
      $defaults = $controller->defaultSettings();
      if (isset($defaults[$name])) {
        return $defaults[$name];
      }
    }
  }

  /**
   * Returns the translator for this job.
   *
   * @return TMGMTTranslator
   *   The translator entity or FALSE if there was a problem.
   */
  public function getTranslator() {
    if (isset($this->translator)) {
      return tmgmt_translator_load($this->translator);
    }
    returN FALSE;
  }

  /**
   * Returns the state of the job. Can be one of the job state constants.
   *
   * @return integer
   *   The state of the job or NULL if it hasn't been set yet.
   */
  public function getState() {
    // We don't need to check if the state is actually set because we always set
    // it in the constructor.
    return $this->state;
  }

  /**
   * Updates the state of the job.
   *
   * @param $state
   *   The new state of the job. Has to be one of the job state constants.
   * @param $message
   *   (Optional) The log message to be saved along with the state change.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   *
   * @return int
   *   The updated state of the job if it could be set.
   *
   * @see TMGMTJob::addMessage()
   */
  public function setState($state, $message = NULL, $variables = array(), $type = 'debug') {
    // Return TRUE if the state could be set. Return FALSE otherwise.
    if (array_key_exists($state, tmgmt_job_states())) {
      $this->state = $state;
      $this->save();
      // If a message is attached to this state change add it now.
      if (isset($message)) {
        $this->addMessage($message, $variables, $type);
      }
    }
    return $this->state;
  }

  /**
   * Checks whether the passed value matches the current state.
   *
   * @param $state
   *   The value to check the current state against.
   *
   * @return boolean
   *   TRUE if the passed state matches the current state, FALSE otherwise.
   */
  public function isState($state) {
    return $this->getState() == $state;
  }

  /**
   * Checks whether the user described by $account is the author of this job.
   *
   * @param $account
   *   (Optional) A user object. Defaults to the currently logged in user.
   */
  public function isAuthor($account = NULL) {
    $account = isset($account) ? $account : $GLOBALS['user'];
    return $this->uid == $account->uid;
  }

  /**
   * Returns whether the state of this job is 'unprocessed'.
   *
   * @return boolean
   *   TRUE if the state is 'unprocessed', FALSE otherwise.
   */
  public function isUnprocessed() {
    return $this->isState(TMGMT_JOB_STATE_UNPROCESSED);
  }

  /**
   * Returns whether the state of this job is 'cancelled'.
   *
   * @return boolean
   *   TRUE if the state is 'cancelled', FALSE otherwise.
   */
  public function isCancelled() {
    return $this->isState(TMGMT_JOB_STATE_CANCELLED);
  }

  /**
   * Returns whether the state of this job is 'active'.
   *
   * @return boolean
   *   TRUE if the state is 'active', FALSE otherwise.
   */
  public function isActive() {
    return $this->isState(TMGMT_JOB_STATE_ACTIVE);
  }

  /**
   * Returns whether the state of this job is 'rejected'.
   *
   * @return boolean
   *   TRUE if the state is 'rejected', FALSE otherwise.
   */
  public function isRejected() {
    return $this->isState(TMGMT_JOB_STATE_REJECTED);
  }

  /**
   * Returns whether the state of this jon is 'finished'.
   *
   * @return boolean
   *   TRUE if the state is 'finished', FALSE otherwise.
   */
  public function isFinished() {
    return $this->isState(TMGMT_JOB_STATE_FINISHED);
  }

  /**
   * Checks whether a job is translatable.
   *
   * @return boolean
   *   TRUE if the job can be translated, FALSE otherwise.
   */
  public function isTranslatable() {
    if ($translator = $this->getTranslator()) {
      if ($translator->canTranslate($this)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Checks whether a job is cancelable.
   *
   * @return boolean
   *   TRUE if the job can be cancelled, FALSE otherwise.
   */
  public function isCancelable() {
    // Only non-submitted translation jobs can be cancelled.
    return $this->isActive();
  }

  /**
   * Checks whether a job is submittable.
   *
   * @return boolean
   *   TRUE if the job can be submitted, FALSE otherwise.
   */
  public function isSubmittable() {
    return $this->isUnprocessed() || $this->isRejected() || $this->isCancelled();
  }

  /**
   * Checks whether a job is deletable.
   *
   * @return boolean
   *   TRUE if the job can be deleted, FALSE otherwise.
   */
  public function isDeletable() {
    return !$this->isActive();
  }

  /**
   * Set the state of the job to 'submitted'.
   *
   * @param $message
   *   The log message to be saved along with the state change.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   *
   * @return TMGMTJob
   *   The job entity.
   *
   * @see TMGMTJob::addMessage()
   */
  public function submitted($message = NULL, $variables = array(), $type = 'status') {
    if (!isset($message)) {
      $message = 'The translation job has been submitted.';
    }
    $this->setState(TMGMT_JOB_STATE_ACTIVE, $message, $variables, $type);
  }

  /**
   * Set the state of the job to 'finished'.
   *
   * @param $message
   *   The log message to be saved along with the state change.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   *
   * @return TMGMTJob
   *   The job entity.
   *
   * @see TMGMTJob::addMessage()
   */
  public function finished($message = NULL, $variables = array(), $type = 'status') {
    if (!isset($message)) {
      $message = 'The translation job has been finished.';
    }
    return $this->setState(TMGMT_JOB_STATE_FINISHED, $message, $variables, $type);
  }

  /**
   * Sets the state of the job to 'cancelled'.
   *
   * @param $message
   *   The log message to be saved along with the state change.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   *
   * Use TMGMTJob::cancelTranslation() to cancel a translation.
   *
   * @return TMGMTJob
   *   The job entity.
   *
   * @see TMGMTJob::addMessage()
   */
  public function cancelled($message = NULL, $variables = array(), $type = 'status') {
    if (!isset($message)) {
      $message = 'The translation job has been cancelled.';
    }
    return $this->setState(TMGMT_JOB_STATE_CANCELLED, $message, $variables, $type);
  }

  /**
   * Sets the state of the job to 'rejected'.
   *
   * @param $message
   *   The log message to be saved along with the state change.
   * @param $variables
   *   (Optional) An array of variables to replace in the message on display.
   *
   * @return TMGMTJob
   *   The job entity.
   *
   * @see TMGMTJob::addMessage()
   */
  public function rejected($message = NULL, $variables = array(), $type = 'error') {
    if (!isset($message)) {
      $message = 'The translation job has been rejected by the translation provider.';
    }
    return $this->setState(TMGMT_JOB_STATE_REJECTED, $message, $variables, $type);
  }

  /**
   * Request the translation of a job from the translator.
   *
   * @return integer
   *   The updated job status.
   */
  public function requestTranslation() {
    if (!$this->isTranslatable() || !$controller = $this->getTranslatorController()) {
      return FALSE;
    }
    // We don't know if the translator plugin already processed our
    // translation request after this point. That means that the plugin has to
    // set the 'submitted', 'needs review', etc. states on its own.
    $controller->requestTranslation($this);
  }

  /**
   * Attempts to cancel the translation job. Already accepted jobs can not be
   * cancelled, submitted jobs only if supported by the translator plugin.
   * Always use this method if you want to cancel a translation job.
   *
   * @return boolean
   *   TRUE if the translation job was cancelled, FALSE otherwise.
   */
  public function cancelTranslation() {
    if (!$this->isCancelable() || !$controller = $this->getTranslatorController()) {
      return FALSE;
    }
    // We don't know if the translator plugin was able to cancel the translation
    // job after this point. That means that the plugin has to set the
    // 'cancelled' state on its own.
    $controller->cancelTranslation($this);
  }

  /**
   * Returns the translator plugin controller of the translator of this job.
   *
   * @return TMGMTTranslatorPluginControllerInterface
   *   The controller of the translator plugin.
   */
  public function getTranslatorController() {
    if ($translator = $this->getTranslator($this)) {
      return $translator->getController();
    }
    return FALSE;
  }

  /**
   * Returns the source data of all job items.
   *
   * @param $reset
   *   Whether the source data should be regenerated from the source objects.
   *
   * @return array
   *   A nested array with the source data where the most upper key is the job
   *   item id.
   */
  public function getSourceData($reset = FALSE) {
    $data = array();
    foreach ($this->getItems() as $key => $item) {
      $data[$key] = $item->getSourceData($reset);
    }
    return $data;
  }

  /**
   * Store translated data back into the items.
   *
   * @param $data
   *   Partially or complete translated data, the most upper key needs to be
   *   the translation job item id.
   * @param $key
   *   (Optional) Either a flattened key (a 'key1][key2][key3' string) or a nested
   *   one, e.g. array('key1', 'key2', 'key2'). Defaults to an empty array which
   *   means that it will replace the whole translated data array. The most
   *   upper key entry needs to be the job id (tjiid).
   */
  public function addTranslatedData($data, $key = NULL) {
    if (empty($key)) {
      $key = array();
    }
    if (!is_array($key)) {
      $key = explode(TMGMT_ARRAY_DELIMITER, $key);
    }
    $items = $this->getItems();
    // If there is a key, get the specific item and forward the call.
    if (!empty($key)) {
      $item_id = array_shift($key);
      if (isset($items[$item_id])) {
        $items[$item_id]->addTranslatedData($data, $key);
      }
    }
    else {
      foreach ($data as $key => $value) {
        if (isset($items[$key])) {
          $items[$key]->addTranslatedData($value);
        }
      }
    }
  }

  /**
   * Retrieves the translated data from a job item.
   *
   * @return
   *   The array of translated data or FALSE if no translated data has been
   *   saved yet.
   */
  public function getTranslatedData() {
    $data = array();
    foreach ($this->getItems() as $item) {
      // Only create an array key if the item actually returned any translated
      // data.
      if ($item_data = $item->getTranslatedData()) {
        $data[$item->tjiid] = $item_data;
      }
    }
    return $data;
  }

  /**
   * Propagates the returned job item translations to the sources.
   *
   * @return boolean
   *   TRUE if we were able to propagate the translated data, FALSE otherwise.
   */
  public function acceptTranslation() {
    foreach ($this->getItems() as $item) {
      $item->acceptTranslation();
    }
  }

}

/**
 * Entity class for the tmgmt_message entity.
 *
 * @ingroup tmgmt_job
 */
class TMGMTMessage extends Entity {

  /**
   * The ID of the message..
   *
   * @var integer
   */
  public $mid;

  /**
   * The ID of the job.
   *
   * @var integer
   */
  public $tjid;

  /**
   * The ID of the job item.
   *
   * @var integer
   */
  public $tjiid;

  /**
   * The message text.
   *
   * @var string
   */
  public $message;

  /**
   * An array of string replacement arguments as used by t().
   *
   * @var array
   */
  public $variables;

  /**
   * The time when the message object was created as a timestamp.
   *
   * @var integer
   */
  public $created;

  /**
   * Type of the message (debug, status, warning or error).
   *
   * @var string
   */
  public $type;

  /**
   * Overrides Entity::__construct().
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'tmgmt_message');
    if (empty($this->created)) {
      $this->created = REQUEST_TIME;
    }
    if (empty($this->type)) {
      $this->type = 'status';
    }
  }

  /**
   * Overrides Entity::label().
   */
  public function defaultLabel() {
    $created = format_date($this->created);
    switch ($this->type) {
      case 'error':
        return t('Error message from @time', array('@time' => $created));
      case 'status':
        return t('Status message from @time', array('@time' => $created));
      case 'warning':
        return t('Warning message from @time', array('@time' => $created));
      case 'debug':
        return t('Debug message from @time', array('@time' => $created));
    }
  }

  /**
   * Returns the translated message.
   *
   * @return
   *   The translated message.
   */
  public function getMessage() {
    $text = $this->message;
    if (is_array($this->variables) && !empty($this->variables)) {
      $text = t($text, $this->variables);
    }
    return $text;
  }

  /**
   * Loads the job entity that this job message is attached to.
   *
   * @return TMGMTJob
   *   The job entity that this job message is attached to or FALSE if there was
   *   a problem.
   */
  public function getJob() {
    if (!empty($this->tjid)) {
      return tmgmt_job_load($this->tjid);
    }
    return FALSE;
  }

  /**
   * Loads the job entity that this job message is attached to.
   *
   * @return TMGMTJobItem
   *   The job item entity that this job message is attached to or FALSE if
   *   there was a problem.
   */
  public function getJobItem() {
    if (!empty($this->tjiid)) {
      return tmgmt_job_item_load($this->tjiid);
    }
    return FALSE;
  }

}
