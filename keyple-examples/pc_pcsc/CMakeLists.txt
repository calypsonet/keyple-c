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
IF(MSVC)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(MSVC)
 
# add the paths for include files
# applications paths
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include")

# SDK paths
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/keyple-core/include")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/keyple-reader-drivers/pcsc/")

# add the executable
ADD_EXECUTABLE(ExamplePCSC 
			   "${CMAKE_SOURCE_DIR}/keyple-core/src/keyple.c"
			   "${CMAKE_SOURCE_DIR}/keyple-core/src/keyple_serequest.c"
			   "${CMAKE_SOURCE_DIR}/keyple-core/src/keyple_serequest_pool.c"
			   "${CMAKE_SOURCE_DIR}/keyple-reader-drivers/pcsc/pcsc_reader.c"
			   "${CMAKE_CURRENT_SOURCE_DIR}/src/log.c"
			   "${CMAKE_CURRENT_SOURCE_DIR}/src/demo_app_main.c"
			   )

# add PC/SC lib
IF(WIN32)
TARGET_LINK_LIBRARIES(ExamplePCSC winscard)
ENDIF(WIN32)

IF(UNIX)
TARGET_LINK_LIBRARIES(ExamplePCSC pcsclite)
ENDIF(UNIX)
