/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#include <winscard.h>
#include <stdio.h>
#include "keyple.h"

#include "pcsc_reader.h"

struct pcsc_reader_s
{
    struct reader_s reader;

    /*! PCSC specific parameters */
    SCARDCONTEXT ctx;
    SCARDHANDLE hcard;
    char name[32];
    DWORD share_mode;
    DWORD prefered_protocols;
    DWORD active_protocol;
};

static SCARDCONTEXT pcsc_ctx = 0;

static int pcsc_openchannel(struct reader_s *rd, uint8_t atr[32], uint16_t *atrlen);
static int pcsc_closechannel(struct reader_s *rd, uint8_t close_type);
static uint8_t *pcsc_getcardinfo(struct reader_s *rd);
static int pcsc_transmit(struct reader_s *rd, uint8_t *cmd, uint16_t cmdlen, uint8_t *rsp, uint16_t rspmax);

/*!
 * \struct  reader_ops_s
 *
 * \brief   A reader ops.
 */

const struct reader_ops_s pcsc_functions = { pcsc_openchannel,
                                             pcsc_closechannel,
                                             pcsc_getcardinfo,
                                             NULL, /* Batch execution is not supported */
                                             pcsc_transmit };

/*!
 * \fn  int pcsc_init()
 *
 * \brief   Pcsc initialize
 *
 * \return  An int.
 */

int pcsc_init()
{
    enum kpl_errors_e ret = KPL_SUCCESS;
    LONG pcsc_status;

    /// Establish the PC/SC context.
    pcsc_status = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &pcsc_ctx);
    if (SCARD_S_SUCCESS != pcsc_status)
    {
        ret = KPL_RDR_INIT_ERROR;
    }
    return ret;
}

/*!
 * \fn  static int pcsc_openchannel(struct reader_s *rd, uint8_t atr[32], uint16_t *atrlen)
 *
 * \brief   Pcsc openchannel
 *
 * \param [in,out]  rd      If non-null, the rd.
 * \param           atr     The atr.
 * \param [in,out]  atrlen  If non-null, the atrlen.
 *
 * \return  An int.
 */

static int pcsc_openchannel(struct reader_s *rd, uint8_t atr[32], uint16_t *atrlen)
{
    enum kpl_errors_e ret;
    struct pcsc_reader_s *pcsc = (struct pcsc_reader_s *)rd;
    LONG pcsc_status;
    if (rd == NULL)
    {
        return KPL_ERROR_NULL_POINTER;
    }
    pcsc_status = SCardConnect(
        pcsc_ctx, pcsc->name, pcsc->share_mode, pcsc->prefered_protocols, &pcsc->hcard, &pcsc->active_protocol);
    if (pcsc_status == SCARD_S_SUCCESS)
    {
        DWORD cbyte = 32;
        /// Get the ATR buffer as attribute
        pcsc_status = SCardGetAttrib(pcsc->hcard, SCARD_ATTR_ATR_STRING, atr, &cbyte);
        if (pcsc_status == SCARD_S_SUCCESS)
        {
            *atrlen = (uint16_t)cbyte;
            KEYPLE_LOGVAL(keyple_ctx, "ATR: ", 0);
            KEYPLE_LOGBUF(keyple_ctx, atr, *atrlen);
            ret = KPL_SUCCESS;
        }
        else
        {
            printf("status = %X\n", pcsc_status);
            ret = KPL_RDR_CARD_INFO_ERROR;
        }
    }
    else
    {
        ret = KPL_RDR_CARD_CONNECTION_ERROR;
    }
    KEYPLE_LOGVAL(keyple_ctx, "status = ", ret);
    return (int)ret;
}

/*!
 * \fn  static int pcsc_closechannel(struct reader_s *rd, uint8_t close_type)
 *
 * \brief   Pcsc closechannel
 *
 * \param [in,out]  rd          If non-null, the rd.
 * \param           close_type  Type of the close.
 *
 * \return  An int.
 */

static int pcsc_closechannel(struct reader_s *rd, uint8_t close_type)
{
    enum kpl_errors_e ret      = KPL_RDR_CARD_DISCONNECTION_ERROR;
    struct pcsc_reader_s *pcsc = (struct pcsc_reader_s *)rd;
    if (SCardDisconnect(pcsc->hcard, close_type) == SCARD_S_SUCCESS)
    {
        ret = KPL_SUCCESS;
    }
    KEYPLE_LOGVAL(keyple_ctx, "status = %d\n", ret);
    return ret;
}

/*!
 * \fn  static uint8_t *pcsc_getcardinfo(struct reader_s *rd)
 *
 * \brief   Pcsc getcardinfo
 *
 * \param [in,out]  rd  If non-null, the rd.
 *
 * \return  Null if it fails, else a pointer to an uint8_t.
 */

static uint8_t *pcsc_getcardinfo(struct reader_s *rd)
{
    struct pcsc_reader_s *pcsc = (struct pcsc_reader_s *)rd;
    return NULL;
}

/*!
 * \fn  static int pcsc_transmit(struct reader_s *rd, uint8_t *cmd, uint16_t cmdlen, uint8_t *rsp, uint16_t rspmax)
 *
 * \brief   Pcsc transmit
 *
 * \param [in,out]  rd      If non-null, the rd.
 * \param [in,out]  cmd     If non-null, the command.
 * \param           cmdlen  The cmdlen.
 * \param [in,out]  rsp     If non-null, the rsp.
 * \param           rspmax  The rspmax.
 *
 * \return  An int.
 */

static int pcsc_transmit(struct reader_s *rd, uint8_t *cmd, uint16_t cmdlen, uint8_t *rsp, uint16_t rspmax)
{
    int ret                    = KPL_RDR_CARD_TRANSMIT_ERROR;
    struct pcsc_reader_s *pcsc = (struct pcsc_reader_s *)rd;
    LONG pcscStatus            = 0;
    DWORD response_length;
    SCARD_IO_REQUEST scioReq;

    scioReq.dwProtocol  = pcsc->active_protocol;
    scioReq.cbPciLength = sizeof(scioReq);

    response_length = (unsigned long)rspmax;

    KEYPLE_LOGTXT(keyple_ctx, "CMD: ");
    KEYPLE_LOGBUF(keyple_ctx, cmd, cmdlen);

    pcscStatus = SCardTransmit(pcsc->hcard, &scioReq, cmd, cmdlen, NULL, rsp, &response_length);

    if (pcscStatus == SCARD_S_SUCCESS)
    {
        KEYPLE_LOGTXT(keyple_ctx, "RESP: ");
        KEYPLE_LOGBUF(keyple_ctx, rsp, response_length);

        if (response_length == 2 && (rsp[0] == 0x61 || rsp[0] == 0x6C || rsp[0] == 0x9F))
        {
            /// handle outgoing data: sw1sw2 = 0x61xx, 0x6Cxx or 0x9Fxx
            uint8_t getResponse[] = { 0x00, 0xC0, 0x00, 0x00, rsp[1] };

            response_length = (unsigned long)rspmax;

            KEYPLE_LOGTXT(keyple_ctx, "CMD: ");
            KEYPLE_LOGBUF(keyple_ctx, getResponse, 5);

            pcscStatus = SCardTransmit(pcsc->hcard, &scioReq, getResponse, 5, NULL, rsp, &response_length);

            if (pcscStatus == SCARD_S_SUCCESS)
            {
                KEYPLE_LOGTXT(keyple_ctx, "RESP: ");
                KEYPLE_LOGBUF(keyple_ctx, rsp, response_length);
                ret = (int)response_length;
            }
        }
        else
        {
            ret = (int)response_length;
        }
    }
    return ret;
}

/* Initialize a PCSC reader given its name */
struct reader_s *pcsc_init_reader(const char *readername, uint16_t readerid)
{
    struct pcsc_reader_s *rd = malloc(sizeof(struct pcsc_reader_s));

    if (rd != NULL)
    {
        rd->reader.ops         = (struct reader_ops_s *)&pcsc_functions;
        rd->reader.ident       = readerid;
        rd->reader.opened      = false;
        rd->share_mode         = SCARD_SHARE_EXCLUSIVE;
        rd->prefered_protocols = SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1;
        strcpy(rd->name, readername);
    }
    return (struct reader_s *)rd;
}

int pcsc_init_allreaders()
{
    //scardlisteaders
    //in the loop, add each reader to the sdk
    return 0;
}
