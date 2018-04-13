/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#ifndef PCSC_READER_H
#define PCSC_READER_H

int pcsc_init();
struct reader_s *pcsc_init_reader(const char *readername, uint16_t readerid);

#endif // PCSC_READER_H
