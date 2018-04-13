/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#include <stdio.h>

#include "keyple.h"
#include "keyple_serequest.h"
#include "keyple_serequest_pool.h"

#include "log.h"
#include "pcsc_reader.h"

#define PO_READER_NAME "ASK LoGO 0"
#define CSM_READER_NAME "SCM Microsystems Inc. SCR35xx USB Smart Card Reader 0"

#define PO_READER_ID 0
#define CSM_READER_ID 1

#define POOL_INDEX_0 0

#define CLA 0
#define INS 1
#define P1 2
#define P2 3
#define P3 4

struct reader_s *po_reader, *csm_reader;

static int doTransaction(void)
{
    return 0;
}

int main(void)
{
    uint8_t pool_buffer[512];
    struct apdupool_s pool;
    struct serequest_params_s se_req;
    struct apdu_command_s *cmd;
    struct apdu_response_s *resp;

    // keyple global init
    keyple_init(&keyple_ctx);

    keyple_ctx.logs = &log_functions;

    // readers init
    pcsc_init();

    po_reader  = pcsc_init_reader(PO_READER_NAME, PO_READER_ID);
    csm_reader = pcsc_init_reader(CSM_READER_NAME, CSM_READER_ID);

    keyple_addreader(&keyple_ctx, po_reader);
    keyple_addreader(&keyple_ctx, csm_reader);

    // request pool init
    serequest_define_pool(&pool, pool_buffer, sizeof(pool_buffer));
    serequest_reset_pool(&pool);

    // request init
    serequest_init(&se_req);

    // ADPU 1
    cmd = serequest_alloccommand(&pool, 5);

    cmd->data[0] = 0xFF;
    cmd->data[1] = 0xCA;
    cmd->data[2] = 0x00;
    cmd->data[3] = 0x00;
    cmd->data[4] = 0x00;

    cmd->len = 5;

    resp = serequest_allocresponse(&pool, 100);

    serequest_addapdu(&se_req, cmd, resp);

    // APDU 2
    cmd = serequest_alloccommand(&pool, 5);

    cmd->data[0] = 0xFF;
    cmd->data[1] = 0xD0;
    cmd->data[2] = 0x00;
    cmd->data[3] = 0x00;
    cmd->data[4] = 0x00;

    cmd->len = 5;

    resp = serequest_allocresponse(&pool, 100);

    serequest_addapdu(&se_req, cmd, resp);

    // execute the 2 APDUs
    uint8_t atr[32];
    uint16_t atrlen;
    while (po_reader->ops->openchannel(po_reader, atr, &atrlen) != KPL_SUCCESS)
    {
        ;
    }
    serequest_execute(&keyple_ctx, po_reader, &se_req);

    printf("Press a key to exit.");
    getchar();
    return 0;
}
