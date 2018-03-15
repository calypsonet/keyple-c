CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
# build the keyple_config.h template
# this file can be used as a base configuration file for the application
configure_file (
  "${PROJECT_SOURCE_DIR}/keyple-core/templates/keyple_config_template.h.in"
  "${PROJECT_SOURCE_DIR}/keyple-core/include/keyple_config_template.h"
)