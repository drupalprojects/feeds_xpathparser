<?php

/**
 * @files
 * Provides the FeedsXPathParserHTML class.
 */

// Require base class.
require_once(dirname(__FILE__) .'/FeedsXPathParserBase.inc');
require_once(dirname(__FILE__) .'/FeedsXPathParserDOMXPath.inc');
require_once(dirname(__FILE__) .'/FeedsXPathParserQueryParser.inc');

/**
 *
 */
class FeedsXPathParserHTML extends FeedsXPathParserBase {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    parent::parse($batch, $source);

    // Determine if we should tidy things up.
    if (!empty($this->source_config['exp']['tidy'])) {
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
      $encoding = $this->source_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($batch->getRaw()), $config, $encoding);
    }
    else {
      $raw = $batch->getRaw();
    }
    $this->doc = new DOMDocument();
    // Use our own error handling.
    $use = $this->errorStart();
    $success = $this->doc->loadHTML($raw);
    unset($raw);
    $this->errorStop($use, $this->source_config['exp']['errors']);
    if (!$success) {
      throw new Exception(t('There was an error parsing the HTML document.'));
    }
    // Parse!
    $this->query($batch);
    unset($this->doc);
  }
}
