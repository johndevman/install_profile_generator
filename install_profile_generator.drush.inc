<?php
/**
 * @file drush commands for install_profile_generator.
 */

/**
 * Implements hook_drush_command().
 */
function install_profile_generator_drush_command() {
  $commands = [];

  $commands['install-profile-generate'] = [
    'Description' => 'Generate an installation profile from the current Drupal site',
    'aliases' => ['ipg'],
    'options' => [
      'name' => 'The name of your install profile',
      'machine_name' => 'The machine name of your install profile',
      'description' => 'The description of your install profile',
    ],
  ];

  return $commands;
}

/**
 * Callback for drush install-profile-generate.
 *
 * TODO summarise what the function does.
 */
function drush_install_profile_generator_install_profile_generate() {
  $name = drush_get_option('name');
  $machine_name = drush_get_option('machine_name');
  $description = drush_get_option('description');

  if ($name && !$machine_name) {
    // Generate machine name from name.
  }

  if ($machine_name && !$name) {
    // Generate name from machine name.
  }

  if (!install_profile_generator_validate($name, $machine_name, $description)) {
    // Explain & bomb out.
  }

  if (!install_profile_generator_confirm($name, $machine_name, $description)) {
    // Bomb out.
  }

  // Do stuff.
}

/**
 *
 */
function install_profile_generator_validate($name, $machine_name, $description) {
  return TRUE;
}

/**
 *
 */
function install_profile_generator_confirm($name, $machine_name, $description) {
  return TRUE;
}

/**
 *
 */
function install_profile_generator_extension_in_current_profile() {
  $has_extension_in_current_profile = FALSE;

  $profile = \Drupal::installProfile();
  $modules = \Drupal::moduleHandler()->getModuleList();

  $profile_path = $modules[$profile]->getPath();

  unset($modules[$profile]);

  foreach ($modules as $module) {
    if (strpos($module->getPath(), $profile_path) === 0) {
      $has_extension_in_current_profile = TRUE;
    }
  }

  $themes = \Drupal::service('theme_handler')->listInfo();
  foreach ($themes as $theme) {
    if (strpos($theme->getPath(), $profile_path) === 0) {
      $has_extension_in_current_profile = TRUE;
    }
  }
  return $has_extension_in_current_profile;
}
