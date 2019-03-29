<?php

/**
 * @file
 * Generate estimates to do migrations.
 */

/**
* Implements hook_drush_command().
*/
function migration_estimates_drush_command() {
  $items['get-migration-estimate'] = array(
    'description' => 'Get estimation to migrate current site.',
    'aliases' => array('gme'),
  );
  $items['get-content-types'] = array(
    'description' => 'Get content types in the site.',
    'aliases' => array('gct'),
  );

  return $items;
}

/**
 *
 */
function drush_migration_estimates_get_migration_estimate() {

  drush_migration_estimates_get_modules();
  drush_migration_estimates_get_content_types();
  drush_migration_estimates_get_views();
  drush_migration_estimates_get_blocks();

}

/**
 *
 */
function drush_migration_estimates_get_views() {
  $list_views = views_get_enabled_views();

  foreach ($list_views as $index => $list_view) {
    echo "view: " . $index . PHP_EOL;
  }

  echo "Number of views in use: " . sizeof($list_views) . PHP_EOL;

  return sizeof($list_views);
}

/**
 *
 */
function drush_migration_estimates_get_modules() {
  $enabled_modules = module_list();

  echo "Number of modules enabled: " . sizeof($enabled_modules) . PHP_EOL;
  return sizeof($enabled_modules);
}

/**
 *
 */
function drush_migration_estimates_get_blocks() {
  module_load_include('inc', 'block', 'block.admin');
  global $theme_key;
  $blocks = block_admin_display_prepare_blocks($theme_key);

  echo "Number of blocks used: " . sizeof($blocks) . PHP_EOL;

  return sizeof($blocks);
}

/**
 * Get content types estimations
 *
 * @return int
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
      $number_fields = sizeof($fields);

    }

    echo "Number of nodes: " . $number_nodes . " - number of fields: " . $number_fields . ": Content type: $content_type_name" . PHP_EOL;
  }

return sizeof($content_types);
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

/**
 * Get final estimations.
 *
 * Formula:
 *   Number of CT*CT_COST + Number of blocks + Number modules enabled + number enabled views
 */
function estimate_hours() {
  $total_complexity_hours_min = 0;
  $total_complexity_hours_max = 0;
  $number_content_types = drush_migration_estimates_get_content_types();

  // Analysis.
  if ($number_content_types < 5) {
    $total_complexity_hours_min = $total_complexity_hours_min + 16;
    $total_complexity_hours_max = $total_complexity_hours_max + 24;
  } else if ($number_content_types < 10) {
    $total_complexity_hours_min = $total_complexity_hours_min + 30;
    $total_complexity_hours_max = $total_complexity_hours_max + 40;
  } else {
    $total_complexity_hours_min = $total_complexity_hours_min + 50 + ($number_content_types * 10);
    $total_complexity_hours_max = $total_complexity_hours_max + 60 + ($number_content_types * 10);
  }

  // Content creation and export.
  if ($number_content_types < 5) {
    $total_complexity_hours_min = $total_complexity_hours_min + 16;
    $total_complexity_hours_max = $total_complexity_hours_max + 40;
  } else if ($number_content_types < 10) {
    $total_complexity_hours_min = $total_complexity_hours_min + 40;
    $total_complexity_hours_max = $total_complexity_hours_max + 80;
  } else {
    $total_complexity_hours_min = $total_complexity_hours_min + $number_content_types * 8;
    $total_complexity_hours_max = $total_complexity_hours_min + $number_content_types * 8;
  }

  // Testing.
  if ($number_content_types < 5) {
    $total_complexity_hours_min =  $total_complexity_hours_min + 24;
    $total_complexity_hours_max = $total_complexity_hours_max + 32;
  } else if ($number_content_types < 10) {
    $total_complexity_hours_min = $total_complexity_hours_min + 40;
    $total_complexity_hours_max = $total_complexity_hours_max + 56;
  } else {
    $total_complexity_hours_min = $total_complexity_hours_min + $number_content_types * 8;
    $total_complexity_hours_max = $total_complexity_hours_min + $number_content_types * 8;
  }

  echo "Minimum hours: " . $total_complexity_hours_min . PHP_EOL;
  echo "Maximum hours: " . $total_complexity_hours_max . PHP_EOL;

}
