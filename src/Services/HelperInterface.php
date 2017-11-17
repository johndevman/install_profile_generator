<?php

namespace Drupal\install_profile_generator\Services;

/**
 * Interface HelperInterface.
 */
interface HelperInterface {

  /**
   * Converts a name to a valid machine name.
   *
   * @param string $name
   *   A name to turn into a valid machine name.
   *
   * @return string
   *   A valid machine name.
   */
  public function convertToMachineName($name);

  /**
   * Validate whether we should proceed with generation of install profile.
   *
   * @param string $name
   *   Name of the new install profile.
   * @param string $machine_name
   *   Machine name of the new install profile.
   *
   * @return bool
   *   TRUE - proceed with generation of install profile.
   *   FALSE - do not proceed with generation of install profile.
   */
  public function validate($name, $machine_name);

}
