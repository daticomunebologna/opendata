<?php

/**
 * Interface for exporting to a given file format.
 */
interface TMGMTFileFormatInterface {

  /**
   * Return the file content for the job data.
   *
   * @param $job
   *   The translation job object to be exported.
   *
   * @return
   *   String with the file content.
   */
  function export(TMGMTJob $job);

  /**
   * Validates that the given file is valid and can be imported.
   *
   * @param $imported_file
   *   File path to the file to be imported.
   *
   * @return TMGMTJob
   *   Returns the corresponding translation job entity if the import file is
   *   valid, FALSE otherwise.
   */
  function validateImport($imported_file);

  /**
   * Converts an exported file content back to the translated data.
   *
   * @return
   *   Translated data array.
   */
  function import($imported_file);
}

/**
 * Export to XLIFF format.
 */
class TMGMTFileformatXLIFF extends XMLWriter implements TMGMTFileFormatInterface {

  /**
   * Contains a reference to the currently being exported job.
   *
   * @var TMGMTJob
   */
  protected $job;

  /**
   * Adds a job item to the xml export.
   *
   * @param $item
   *   The job item entity.
   */
  protected function addItem(TMGMTJobItem $item) {
    $this->startElement('group');
    $this->writeAttribute('id', $item->tjiid);

    // Add a note for the source label.
    $this->writeElement('note', $item->getSourceLabel());

    // @todo: Write in nested groups instead of flattening it.
    $data = array_filter(tmgmt_flatten_data($item->getData()), '_tmgmt_filter_data');
    foreach ($data as $key => $element) {
      $this->addTransUnit($item->tjiid . '][' . $key, $element);
    }
    $this->endElement();
  }

  /**
   * Adds a single translation unit for a data element.
   *
   * @param $key
   *   The unique identifier for this data element.
   *
   * @param $element
   *   Array with the properties #text and optionally #label.
   */
  protected function addTransUnit($key, $element) {
    $this->startElement('trans-unit');
    $this->writeAttribute('id', $key);
    $this->writeAttribute('resname', $key);

    $this->startElement('source');
    $this->writeAttribute('xml:lang', $this->job->source_language);
    $this->text($element['#text']);
    $this->endElement();
    $this->startElement('target');
    $this->writeAttribute('xml:lang', $this->job->target_language);
    $this->text($element['#text']);
    $this->endElement();
    if (isset($element['#label'])) {
      $this->writeElement('note', $element['#label']);
    }
    $this->endElement();
  }

  /**
   * Implements TMGMTFileExportInterface::export().
   */
  public function export(TMGMTJob $job) {

    $this->job = $job;

    $this->openMemory();
    $this->setIndent(true);
    $this->setIndentString(' ');
    $this->startDocument('1.0', 'UTF-8');

    // Root element with schema definition.
    $this->startElement('xliff');
    $this->writeAttribute('version', '1.2');
    $this->writeAttribute('xmlns', 'urn:oasis:names:tc:xliff:document:1.2');
    $this->writeAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
    $this->writeAttribute('xsi:schemaLocation', 'urn:oasis:names:tc:xliff:document:1.2 xliff-core-1.2-strict.xsd');

    // File element.
    $this->startElement('file');
    $this->writeAttribute('original', 'xliff-core-1.2-strict.xsd');
    $this->writeAttribute('source-language', $job->source_language);
    $this->writeAttribute('target-language', $job->target_language);
    $this->writeAttribute('datatype', 'plaintext');
    // Date needs to be in ISO-8601 UTC
    $this->writeAttribute('date', date('Y-m-d\Th:m:i\Z'));

    $this->startElement('header');
    $this->startElement('phase-group');
    $this->startElement('phase');
    $this->writeAttribute('tool-id', 'tmgmt');
    $this->writeAttribute('phase-name', 'extraction');
    $this->writeAttribute('process-name', 'extraction');
    $this->writeAttribute('job-id', $job->tjid);

    $this->endElement();
    $this->endElement();
    $this->startElement('tool');
    $this->writeAttribute('tool-id', 'tmgmt');
    $this->writeAttribute('tool-name', 'Drupal Translation Management Tools');
    $this->endElement();
    $this->endElement();

    $this->startElement('body');

    foreach ($job->getItems() as $item) {
      $this->addItem($item);
    }

    // End the body, file and xliff tags.
    $this->endElement();
    $this->endElement();
    $this->endElement();
    $this->endDocument();
    return $this->outputMemory();
  }

  /**
   * Implements TMGMTFileExportInterface::import().
   */
  public function import($imported_file) {
    // It is not possible to load the file directly with simplexml as it gets
    // url encoded due to the temporary://. This is a PHP bug, see
    // https://bugs.php.net/bug.php?id=61469
    $xml_string = file_get_contents($imported_file);
    $xml = simplexml_load_string($xml_string);

    // Register the xliff namespace, required for xpath.
    $xml->registerXPathNamespace('xliff', 'urn:oasis:names:tc:xliff:document:1.2');

    $data = array();
    foreach ($xml->xpath('//xliff:trans-unit') as $unit) {
      $data[(string) $unit['id']]['#text'] = (string) $unit->target;
    }
    return tmgmt_unflatten_data($data);
  }

  /**
   * Implements TMGMTFileExportInterface::validateImport().
   */
  public function validateImport($imported_file) {
    // It is not possible to load the file directly with simplexml as it gets
    // url encoded due to the temporary://. This is a PHP bug, see
    // https://bugs.php.net/bug.php?id=61469
    $xml_string = file_get_contents($imported_file);
    $xml = simplexml_load_string($xml_string);

    if (!$xml) {
      return FALSE;
    }

    // Register the xliff namespace, required for xpath.
    $xml->registerXPathNamespace('xliff', 'urn:oasis:names:tc:xliff:document:1.2');

    // Check if our phase information is there.
    $phase = $xml->xpath("//xliff:phase[@phase-name='extraction']");
    if ($phase) {
      $phase = reset($phase);
    }
    else {
      return FALSE;
    }

    // Check if the job can be loaded.
    if (!isset($phase['job-id']) || (!$job = tmgmt_job_load((string) $phase['job-id']))) {
      return FALSE;
    }

    // Compare source language.
    if (!isset($xml->file['source-language']) || $job->source_language != $xml->file['source-language']) {
      return FALSE;
    }

    // Compare target language.
    if (!isset($xml->file['target-language']) || $job->target_language != $xml->file['target-language']) {
      return FALSE;
    }

    // Validation successful.
    return $job;
  }

}

/**
 * Export into HTML.
 */
class TMGMTFileFormatHTML implements TMGMTFileFormatInterface {

  /**
   * Returns base64 encoded data that is safe for use in xml ids.
   */
  protected function encodeIdSafeBase64($data) {
    // Prefix with a b to enforce that the first character is a letter.
    return 'b' . rtrim(strtr(base64_encode($data), '+/', '-_'), '=');
  }

  /**
   * Returns decoded id safe base64 data.
   */
  protected function decodeIdSafeBase64($data) {
    // Remove prefixed b.
    $data = substr($data, 1);
    return base64_decode(str_pad(strtr($data, '-_', '+/'), strlen($data) % 4, '=', STR_PAD_RIGHT));
  }

  /**
   * Implements TMGMTFileExportInterface::export().
   */
  public function export(TMGMTJob $job) {
    $items = array();
    foreach ($job->getItems() as $item) {
      $data = array_filter(tmgmt_flatten_data($item->getData()), '_tmgmt_filter_data');
      foreach ($data as $key => $value) {
        $items[$item->tjiid][$this->encodeIdSafeBase64($item->tjiid . '][' . $key)] = $value;
      }
    }
    return theme('tmgmt_file_html_template', array(
      'tjid' => $job->tjid,
      'source_language' => $job->source_language,
      'target_language' => $job->target_language,
      'items' => $items,
    ));
  }

  /**
   * Implements TMGMTFileExportInterface::import().
   */
  public function import($imported_file) {
    $dom = new DOMDocument();
    $dom->loadHTMLFile($imported_file);
    $xml = simplexml_import_dom($dom);

    $data = array();
    foreach ($xml->xpath("//div[@class='atom']") as $atom) {
      // Assets are our strings (eq fields in nodes).
      $key = $this->decodeIdSafeBase64((string) $atom['id']);
      $data[$key]['#text'] = (string) $atom;
    }
    return tmgmt_unflatten_data($data);
  }

  /**
   *
   * @param type $imported_file
   */
  public function validateImport($imported_file) {
    $dom = new DOMDocument();
    if (!$dom->loadHTMLFile($imported_file)) {
      return FALSE;
    }
    $xml = simplexml_import_dom($dom);

    // Collect meta information.
    $meta_tags = $xml->xpath('//meta');
    $meta = array();
    foreach ($meta_tags as $meta_tag) {
      $meta[(string) $meta_tag['name']] = (string) $meta_tag['content'];
    }

    // Check required meta tags.
    foreach (array('JobID', 'languageSource', 'languageTarget') as $name) {
      if (!isset($meta[$name])) {
        return FALSE;
      }
    }

    // Attempt to load job.
    if (!$job = tmgmt_job_load($meta['JobID'])) {
      return FALSE;
    }

    // Check language.
    if ($meta['languageSource'] != $job->source_language || $meta['languageTarget'] != $job->target_language) {
      return FALSE;
    }

    // Validation successful.
    return $job;
  }

}
