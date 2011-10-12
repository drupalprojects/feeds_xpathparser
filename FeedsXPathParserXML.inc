<?php

/**
 * @files
 * Provides the FeedsXPathParserXML class.
 */

/**
 * Parse XML using XPath.
 */
class FeedsXPathParserXML extends FeedsXPathParserBase {

  /**
   * Implementation of FeedsXPathParserBase::setup().
   */
  protected function setup($source_config, FeedsImportBatch $batch) {

    if (!empty($source_config['exp']['tidy'])) {
      $config = array(
        'input-xml' => TRUE,
        'wrap'      => 0,
        'tidy-mark' => FALSE,
      );
      // Default tidy encoding is UTF8.
      $encoding = $source_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($batch->getRaw()), $config, $encoding);
    }
    else {
      $raw = $batch->getRaw();
    }
    $doc = new DOMDocument();
    $use = $this->errorStart();
    $success = $doc->loadXML($raw);
    unset($raw);
    $this->errorStop($use, $source_config['exp']['errors']);
    if (!$success) {
      throw new Exception(t('There was an error parsing the XML document.'));
    }
    return $doc;
  }
}
