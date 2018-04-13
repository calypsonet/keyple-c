#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#
cmake_minimum_required(VERSION 2.8)

# The SDK version number.
set (KEYPLE_SDK_VERSION_MAJOR 0)
set (KEYPLE_SDK_VERSION_MINOR 1)

# configure the SDK
#add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/keyple-core )

# implementation examples
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/keyple-examples/pc_pcsc )

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/keyple-examples/pc_pcsc PROPERTY VS_STARTUP_PROJECT ExamplePCSC)
