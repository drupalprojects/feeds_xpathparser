<?php

/**
 * @file
 * Contains \Drupal\feeds_xpathparser\Plugin\feeds\Parser\FeedsXPathParserHTML.
 */

namespace Drupal\feeds_xpathparser\Plugin\feeds\Parser;

use Drupal\feeds\FetcherResultInterface;
use Drupal\feeds_xpathparser\FeedsXPathParserBase;

/**
 * Parses HTML documents using XPath.
 */
class FeedsXPathParserHTML extends FeedsXPathParserBase {

  /**
   * {@inheritdoc}
   */
  protected function setup(array $feed_config, FetcherResultInterface $fetcher_result) {

    if (!empty($feed_config['exp']['tidy'])) {
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
      $encoding = $feed_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($fetcher_result->getRaw()), $config, $encoding);
    }
    else {
      $raw = $fetcher_result->getRaw();
    }
    $doc = new \DOMDocument();
    // Use our own error handling.
    $use = $this->errorStart();
    $success = $doc->loadHTML($raw);
    unset($raw);
    $this->errorStop($use, $feed_config['exp']['errors']);
    if (!$success) {
      throw new \RuntimeException(t('There was an error parsing the HTML document.'));
    }

    return $doc;
  }

  /**
   * {@inheritdoc}
   */
  protected function getRaw(\DOMNode $node) {
    return $this->doc->saveHTML($node);
  }

}
