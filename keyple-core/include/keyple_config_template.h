/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

/*!
 * \file keyple_config.h.
 *
 * \brief The keyple SDK configuration
 */

#ifndef KEYPLE_CONFIG_H
#define KEYPLE_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

 ///////////////////////////////////////////////////////////////////////.
 // SDK Version
 ///////////////////////////////////////////////////////////////////////.
#define KEYPLE_SDK_VERSION_MAJOR      0  
#define KEYPLE_SDK_VERSION_MINOR      1    

 ///////////////////////////////////////////////////////////////////////.
 // Logging configuration
 ///////////////////////////////////////////////////////////////////////.
#define KPL_LOG_ENABLED
#define KPL_LOG_SPACES_IN_HEXDUMPS  true

 ///////////////////////////////////////////////////////////////////////.
 // Readers configuration
 ///////////////////////////////////////////////////////////////////////.
#define READER_0						0
#define READER_1						1
#define MAX_READER_INDEX				1
#define MAX_NBR_READERS					(MAX_READER_INDEX+1)

 ///////////////////////////////////////////////////////////////////////.
 // Memory management configuration
 ///////////////////////////////////////////////////////////////////////.
#define COMMAND_APDU_POOL_INDEX			0
#define RESPONSE_APDU_POOL_INDEX		1
#define MAX_POOL_INDEX					1
#define MAX_NBR_POOLS					(MAX_POOL_INDEX+1)


#endif //  KEYPLE_CONFIG_H
