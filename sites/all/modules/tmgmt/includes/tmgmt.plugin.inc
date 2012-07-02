<?php

/**
 * @file
 * Contains the classes and interfaces for the services and source plugins.
 */

/**
 * Base interface for Translation Management plugins.
 */
interface TMGMTPluginBaseInterface {

  /**
   * Constructor.
   *
   * @param $type
   *   The plugin type.
   * @param $plugin
   *   The machine-readable name of the plugin.
   */
  public function __construct($type, $plugin);

  /**
   * Returns the info of the type of the plugin.
   *
   * @see tmgmt_source_plugin_info()
   */
  public function pluginInfo();

  /**
   * Returns the type of the plugin.
   */
  public function pluginType();

}

/**
 * Base class for Translation Management plugins.
 */
class TMGMTPluginBase implements TMGMTPluginBaseInterface {

  protected $pluginType;
  protected $pluginInfo;

  /**
   * Implements TMGMTSourcePluginControllerInterface::__construct().
   */
  public function __construct($type, $plugin) {
    $this->pluginType = $plugin;
    $this->pluginInfo = _tmgmt_plugin_info($type, $plugin);
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::pluginInfo().
   */
  public function pluginInfo() {
    return $this->pluginInfo;
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::pluginType().
   */
  public function pluginType() {
    return $this->pluginType;
  }

}

/**
 * Interface for source plugin controllers.
 *
 * @ingroup tmgmt_source
 */
interface TMGMTSourcePluginControllerInterface extends TMGMTPluginBaseInterface {

  /**
   * Returns an array with the data structured for translation.
   *
   * @param TMGMTJobItem $job_item
   *   The job item entity.
   *
   * @see TMGMTJobItem::getSourceData()
   */
  public function getData(TMGMTJobItem $job_item);

  /**
   * Saves a translation.
   *
   * @param TMGMTJobItem $job_item
   *   The job item entity.
   *
   * @return boolean
   *   TRUE if the translation was saved successfully, FALSE otherwise.
   */
  public function saveTranslation(TMGMTJobItem $job_item);

  /**
   * Return a title for this job item.
   *
   * @param TMGMTJobItem $job_item
   *   The job item entity.
   */
  public function getLabel(TMGMTJobItem $job_item);

  /**
   * Returns the Uri for this job item.
   *
   * @param TMGMTJobItem $job_item
   *   The job item entity.
   *
   * @see entity_uri()
   */
  public function getUri(TMGMTJobItem $job_item);

  /**
   * Returns an array of translatable source item types.
   */
  public function getItemTypes();

  /**
   * Returns the label of a source item type.
   *
   * @param $type
   *   The identifier of a source item type.
   */
  public function getItemTypeLabel($type);

}

/**
 * Default controller class for source plugins.
 *
 * @ingroup tmgmt_source
 */
abstract class TMGMTDefaultSourcePluginController extends TMGMTPluginBase implements TMGMTSourcePluginControllerInterface {

  /**
   * Implements TMGMTSourcePluginControllerInterface::getLabel().
   */
  public function getLabel(TMGMTJobItem $job_item) {
    return t('@plugin item unavailable (@item)', array('@plugin' => $this->pluginInfo['label'], '@item' => $job_item->item_type . ':' . $job_item->item_id));
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::getUri().
   */
  public function getUri(TMGMTJobItem $job_item) {
    return FALSE;
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::getItemTypes().
   */
  public function getItemTypes() {
    return isset($this->pluginInfo['item types']) ? $this->pluginInfo['item types'] : array();
  }

  /**
   * Implements TMGMTSourcePluginControllerInterface::getItemTypeLabel().
   */
  public function getItemTypeLabel($type) {
    $types = $this->getItemTypes();
    if (isset($types[$type])) {
      return $types[$type];
    }
    return '';
  }

}

/**
 * Interface for service plugin controllers.
 *
 * @ingroup tmgmt_translator
 */
interface TMGMTTranslatorPluginControllerInterface extends TMGMTPluginBaseInterface {

  /**
   * Checks whether a translator is available.
   *
   * @param TMGMTTranslator $translator
   *   The translator entity.
   *
   * @return boolean
   *   TRUE if the translator plugin is available, FALSE otherwise.
   */
  public function isAvailable(TMGMTTranslator $translator);

  /**
   * Return a reason why the translator is not available.
   *
   * @param TMGMTTranslator $translator
   *   The translator entity.
   *
   * Might be called when isAvailable() returns FALSE to get a reason that
   * can be displayed to the user.
   *
   * @todo Remove this once http://drupal.org/node/1420364 is done.
   */
  public function getNotAvailableReason(TMGMTTranslator $translator);

  /**
   * Check whether this service can handle a particular translation job.
   *
   * @param TMGMTTranslator $translator
   *   The TMGMTTranslator entity that should handle the translation.
   * @param TMGMTJob $job
   *   The TMGMTJob entity that should be translated.
   *
   * @return boolean
   *   TRUE if the job can be processed and translated, FALSE otherwise.
   */
  public function canTranslate(TMGMTTranslator $translator, TMGMTJob $job);

  /**
   * Returns all available target languages that are supported by this service
   * when given a source language.
   *
   * @param TMGMTTranslator $translator
   *   The translator entity.
   * @param $source_language
   *   The source language.
   *
   * @return array
   *   An array of languages in ISO format.
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language);

  /**
   * @abstract
   *
   * Submits the translation request and sends it to the translation provider.
   *
   * @param TMGMTJob $job
   *   The job that should be submitted.
   */
  public function requestTranslation(TMGMTJob $job);

  /**
   * Cancels a translation job.
   *
   * @param TMGMTJob $job
   *   The job that should have its translation cancelled.
   *
   * @return boolean
   *   TRUE if the job could be cancelled, FALSE otherwise.
   */
  public function cancelTranslation(TMGMTJob $job);

  /**
   * Defines default settings.
   *
   * @return array
   *   An array of default settings.
   */
  public function defaultSettings();

  /**
   * Returns if the translator has any settings for the passed job.
   */
  public function hasCheckoutSettings(TMGMTJob $job);

}

/**
 * Default controller class for service plugins.
 *
 * @ingroup tmgmt_translator
 */
abstract class TMGMTDefaultTranslatorPluginController extends TMGMTPluginBase implements TMGMTTranslatorPluginControllerInterface {

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::isAvailable().
   */
  public function isAvailable(TMGMTTranslator $translator) {
    // Assume that the translation service is always available.
    return TRUE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::canTranslate().
   */
  public function canTranslate(TMGMTTranslator $translator, TMGMTJob $job) {
    // The job is only translatable if the translator is available too.
    if ($this->isAvailable($translator) && array_key_exists($job->target_language, $translator->getSupportedTargetLanguages($job->source_language))) {
      // We can only translate this job if the target language of the job is in
      // one of the supported languages.
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::cancelTranslation().
   */
  public function cancelTranslation(TMGMTJob $job) {
    // Assume that we can cancel a translation job at any time.
    $job->setState(TMGMT_JOB_STATE_CANCELLED);
    return TRUE;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::getSupportedTargetLanguages().
   */
  public function getSupportedTargetLanguages(TMGMTTranslator $translator, $source_language) {
    $languages = entity_metadata_language_list();
    unset($languages[LANGUAGE_NONE], $languages[$source_language]);
    return drupal_map_assoc(array_keys($languages));
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::getNotAvailableReason().
   */
  public function getNotAvailableReason(TMGMTTranslator $translator) {
    return t('@translator is not available. Make sure it is properly !configured.', array('@translator' => $this->pluginInfo['label'], '!configured' => l(t('configured'), 'admin/config/regional/tmgmt/translators/manage/' . $translator->name)));
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::defaultSettings().
   */
  public function defaultSettings() {
    $defaults = array('auto_accept' => FALSE);
    // Check if any default settings are defined in the plugin info.
    if (isset($this->pluginInfo['default settings'])) {
      return array_merge($defaults, $this->pluginInfo['default settings']);
    }
    return $defaults;
  }

  /**
   * Implements TMGMTTranslatorPluginControllerInterface::checkoutInfo().
   */
  public function hasCheckoutSettings(TMGMTJob $job) {
    return TRUE;
  }

}
