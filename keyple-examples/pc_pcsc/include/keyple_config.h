/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#ifndef CONFIG_H
#define CONFIG_H

/* Max number of preallocated readers. If more readers are required, use the list of dynamic readers. */
#define CONFIG_KEYPLE_MAX_READERS 2

/* Define this to support an arbitrary number of readers. This will use more memory. */
//#define CONFIG_KEYPLE_DYNAMIC_READERS

/* Define this to enable logging */
//#define CONFIG_KEYPLE_LOGS
#endif
