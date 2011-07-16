<?php

/**
 * @files
 * Provides the FeedsXPathParserXML class.
 */

// Require base class.
require_once(dirname(__FILE__) .'/FeedsXPathParserBase.inc');
require_once(dirname(__FILE__) .'/FeedsXPathParserDOMXPath.inc');
require_once(dirname(__FILE__) .'/FeedsXPathParserQueryParser.inc');

/**
 *
 */
class FeedsXPathParserXML extends FeedsXPathParserBase {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    parent::parse($batch, $source);
    $raw = $batch->getRaw();

    if (empty($raw)) {
      drupal_set_message(t('Feeds XPath parser: The XML document is empty.'), 'warning');
      return;
    }
    if (isset($this->source_config['exp']['tidy']) && $this->source_config['exp']['tidy']) {
      $config = array(
        'input-xml' => TRUE,
        'wrap'      => 0,
        'tidy-mark' => FALSE,
      );
      // Default tidy encoding is UTF8.
      $encoding = $this->source_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($raw), $config, $encoding);
    }
    $this->doc = new DOMDocument();
    $this->doc->loadXML($raw);
    unset($raw);
    $this->query();
    unset($this->doc);
  }
}
