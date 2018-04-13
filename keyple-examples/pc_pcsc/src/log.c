/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#include <stdio.h>

#include "keyple.h"

static void log(char *function, int linenum, char *txt, uint32_t val);
static void logbuf(char *function, int linenum, uint8_t *buf, uint32_t len);

const struct keyple_log_s log_functions = { log, logbuf };

static void log(char *function, int linenum, char *txt, uint32_t val)
{
    printf("%s:%d %s%d\n", function, linenum, txt, val);
}

static void logbuf(char *function, int linenum, uint8_t *buf, uint32_t len)
{
    printf("%s:%d ", function, linenum);
    for (uint32_t ii = 0; ii < len; ii++)
    {
        printf("%02X", buf[ii]);
    }
    printf("\n");
}
