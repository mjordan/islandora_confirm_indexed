<?php

/**
 * @file
 * Drush file for the Islandora Confirm Indexed module.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_confirm_indexed_drush_command() {
  $items = array();
  $items['islandora_confirm_indexed'] = array(
    'description' => 'Queries the Islandora Solr instance for a list of objects and logs ones that are not indexed.',
    'examples' => array(
      "drush islandora_confirm_indexed --pid_file=/tmp/pids.txt --unindexed_log=/tmp/unindexed.log",
    ),
    'options' => array(
      'pid_file' => array(
        'description' => 'Absolute path to a file containing PIDs of objects to delete, one PID per line.',
      ),
      'unindexed_log' => array(
        'description' => 'Absolute path to a file where PIDs of objects not indexed in Solr will be written, one PID per line.',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Purges objects (including page objects).
 */
function drush_islandora_confirm_indexed() {
  if (!module_exists('islandora_solr')) {
    drush_set_error('SOLR_SEARCH_NOT_AVAILABLE',
      dt('Sorry, Islandora Solr Search not enabled.'));
    exit;
  }

  // Check to see if there is a PID file and if there is, use the PIDs
  // from it instead of from Solr.
  if (drush_get_option('pid_file')) {
    $pid_file_path = drush_get_option('pid_file');
    if (!file_exists($pid_file_path)) {
      drush_print(t("The PID file !pid_file doesn't exist", array('!pid_file' => $pid_file_path)));
      drupal_exit();
    }
    $pids = islandora_confirm_indexed_read_pid_file($pid_file_path);
  }

  if (!count($pids) || !$pids) {
    drush_print('There are no matching objects to delete.');
    drupal_exit();
  }

  // Cycle through the objects and query Solr for each one.
  foreach ($pids as $pid) {
    // In case we want to check for active objects only.
    // $object = islandora_object_load($pid);
    list($namespace, $number) = explode(':', trim($pid));
    if (islandora_confirm_indexed_query_solr($namespace, $number)) {
      drush_log(dt("Object !pid is indexed in Solr", array('!pid' => $pid)), 'ok');
    }
    else {
      drush_log(dt("Object !pid is not indexed in Solr", array('!pid' => $pid)), 'warning');
      islandora_confirm_indexed_write_log($pid);
    }
  }
}
