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
  ];

  return $commands;
}

/**
 * Callback for drush install-profile-generate.
 *
 * TODO summarise what the function does.
 */
function drush_install_profile_generator_install_profile_generate() {
}
