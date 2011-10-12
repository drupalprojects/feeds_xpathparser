<?php

/**
 * @files
 * Provides the FeedsXPathParserHTML class.
 */

/**
 * Parse HTML using XPath.
 */
class FeedsXPathParserHTML extends FeedsXPathParserBase {

  /**
   * Implementation of FeedsXPathParserBase::setup().
   */
  protected function setup($source_config, FeedsImportBatch $batch) {

    if (!empty($source_config['exp']['tidy'])) {
      $config = array(
        'merge-divs'       => FALSE,
        'merge-spans'      => FALSE,
        'join-styles'      => FALSE,
        'drop-empty-paras' => FALSE,
        'wrap'             => 0,
        'tidy-mark'        => FALSE,
        'escape-cdata'     => TRUE,
        'word-2000'        => TRUE,
      );
      // Default tidy encoding is UTF8.
      $encoding = $source_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($batch->getRaw()), $config, $encoding);
    }
    else {
      $raw = $batch->getRaw();
    }
    $doc = new DOMDocument();
    // Use our own error handling.
    $use = $this->errorStart();
    $success = $doc->loadHTML($raw);
    unset($raw);
    $this->errorStop($use, $source_config['exp']['errors']);
    if (!$success) {
      throw new Exception(t('There was an error parsing the HTML document.'));
    }
    return $doc;
  }
}
