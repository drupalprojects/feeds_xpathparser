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

    $raw = $batch->getRaw();

    if (empty($raw)) {
      drupal_set_message(t('Feeds XPath parser: The HTML document is empty.'), 'warning');
      return;
    }
    if (isset($this->source_config['exp']['tidy']) && $this->source_config['exp']['tidy']) {
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
      $raw = tidy_repair_string(trim($raw), $config, $encoding);
    }
    $this->doc = new DOMDocument();
    if ($this->source_config['exp']['errors']) {
      $success = $this->doc->loadHTML($raw);
    }
    else {
      $success = @$this->doc->loadHTML($raw);
    }
    unset($raw);
    if (!$success) {
      throw new Exception(t('There was an error parsing the HTML document.'));
    }
    // Parse!
    $this->query();
    unset($this->doc);
    unset($this->batch);
  }
}
