<?php

/**
 * @file
 * Generate estimates to do migrations.
 */

/**
* Implements hook_drush_command().
*/
function migration_estimates_drush_command() {
  $items['get-content-types'] = array(
    'description' => 'Get content types in the site.',
    'aliases' => array('gct'),
  );

  return $items;
}

/**
 * Get content types estimations
 */
function drush_migration_estimates_get_content_types() {

  $content_types = node_type_get_types();
  echo "Number of content types: " . sizeof($content_types) . PHP_EOL;

  foreach ($content_types as $content_type_name => $content_type) {
    $entities = get_content_type($content_type_name);
    $number_nodes = 0;
    if (!empty($entities)) {
      $nodes = node_load_multiple(array_keys($entities['node']));
      $number_nodes = sizeof($nodes);

      $fields = get_content_type_fields($content_type_name);
//      print_r($fields);
      $number_fields = sizeof($fields);

    }

    echo "Number of nodes: " . $number_nodes . " - number of fields: " . $number_fields . ": Content type: $content_type_name" . PHP_EOL;
  }


}

function get_content_type_fields($content_type) {
  $fields = field_info_instances("node", $content_type);

  return $fields;
}

/**
 * @param $content_type_name
 *
 * @return mixed
 */
function get_content_type($content_type_name) {
  $query = new EntityFieldQuery();
  return $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $content_type_name)
    ->execute();
}