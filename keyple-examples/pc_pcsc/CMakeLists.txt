#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(SdkCEmbeddePCPcSc C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# specific settings
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
 
# add the paths for include files
# applications paths
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
# SDK paths
include_directories("${CMAKE_SOURCE_DIR}/keyple-core/include")

# add the executable
add_executable(ExamplePCSC 
			   "${CMAKE_SOURCE_DIR}/keyple-core/src/keyple_reader_interface.c"
			   "${CMAKE_CURRENT_SOURCE_DIR}/src/pcsc_readers.c"
			   "${CMAKE_CURRENT_SOURCE_DIR}/src/demo_app_main.c"
			   "${CMAKE_CURRENT_SOURCE_DIR}/src/log.c"
			   )

# add PC/SC lib
IF(WIN32)
target_link_libraries(ExamplePCSC winscard)
ENDIF(WIN32)

IF(UNIX)
target_link_libraries(ExamplePCSC pcsclite)
ENDIF(UNIX)
