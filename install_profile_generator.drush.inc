<?php

/**
 * @file drush commands for install_profile_generator.
 */

use \Drupal\Core\Language\LanguageInterface;
use \Drupal\Core\Serialization\Yaml;

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
  $name = drush_get_option('name', '');
  $machine_name = drush_get_option('machine_name', '');
  $description = drush_get_option('description', '');

  if ($name && empty($machine_name)) {
    // Generate machine name from name.
    $new_value = \Drupal::transliteration()->transliterate($name, LanguageInterface::LANGCODE_DEFAULT, '_');
    $new_value = strtolower($new_value);
    $new_value = preg_replace('/[^a-z0-9_]+/', '_', $new_value);
    $machine_name = preg_replace('/_+/', '_', $new_value);
  }

  if ($machine_name && empty($name)) {
    // Generate name from machine name.
    $name = $machine_name;
  }

  if (!install_profile_generator_validate($name, $machine_name, $description)) {
    // Exit with an error status.
    return FALSE;
  }

  if (!install_profile_generator_confirm($name, $machine_name, $description)) {
    // The user has chosen to not continue. There's no error.
    return TRUE;
  }

  // Do stuff.
  $profile_path = \DRUPAL::root() . '/profiles/' . $machine_name;
  /** @var \Drupal\Core\File\FileSystemInterface $file_system */
  $file_system = \Drupal::service('file_system');
  if (!$file_system->mkdir($profile_path)) {
    return drush_set_error(
      'INSTALL_GENERATE_PROFILE_CREATE_PROFILE_DIRECTORY',
      dt('Could not create @profile_path directory', ['@profile_path' => $profile_path])
    );
  }
  $info = [
    'name' => $name,
    'type' => 'profile',
    'description' => $description,
    // @todo - why is this not \Drupal::version?
    'core' => '8.x',
  ];
  if (!file_put_contents("$profile_path/$machine_name.info.yml", Yaml::encode($info))) {
    return drush_set_error(
      'INSTALL_GENERATE_PROFILE_CREATE_PROFILE_DIRECTORY',
      dt('Could not write @profile_path/@machine_name.info.yml', ['@profile_path' => $profile_path, '@machine_name' => $machine_name])
    );
  }
}

/**
 *
 */
function install_profile_generator_validate($name, $machine_name, $description) {
  // Check if modules/theme exist in current profile folder.
  if (!install_profile_generator_extension_in_current_profile()) {
    return drush_set_error('INSTALL_GENERATE_PROFILE_EXTENSION_IN_CURRENT_PROFILE', dt('The current profile contains extensions. It is not possible to generate a new profile using Drush.'));
  }

  // Ensure we have a name.
  if (empty($machine_name) || empty($name) || $machine_name === TRUE || $name === TRUE) {
    return drush_set_error('INSTALL_GENERATE_PROFILE_EXTENSION_NO_NAME', dt('To generate a new profile using Drush you have to provide a name or a machine name for the new profile.'));
  }

  // Ensure we won't create a profile with the same name as an existing
  // extension.
  if (\Drupal::moduleHandler()->moduleExists($machine_name) ||
    \Drupal::service('theme_handler')->themeExists($machine_name)
  ) {
    return drush_set_error(
      'INSTALL_GENERATE_PROFILE_MACHINE_NAME_IN_USE',
      dt('The machine name @machine_name already exists', ['@machine_name' => $machine_name])
    );
  }

  // Ensure that the /profiles directory can be written too.
  if (!is_writable(\Drupal::root() . '/profiles')) {
    return drush_set_error('INSTALL_GENERATE_PROFILE_PROFILE_DIRECTORY', dt('Can not write to the /profiles directory'));
  }

  // Ensure we can write to settings.php.
  $settings_file = \Drupal::service('site.path') . '/settings.php';
  if (!is_writeable($settings_file)) {
    return drush_set_error('INSTALL_GENERATE_PROFILE_PROFILE_DIRECTORY', dt('Can not write to @settings', ['@settings' => $settings_file]));
  }
  return TRUE;
}

/**
 *
 */
function install_profile_generator_confirm($name, $machine_name, $description) {
  drush_print(dt('About to generate a new install profile with the machine name "@machine_name"', ['@machine_name' => $machine_name]));
  return drush_confirm(dt('Do it?'));
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
