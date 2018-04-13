/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#include "keyple.h"
#include "keyple_serequest.h"

/*!
 * \fn  int serequest_init(struct serequest_params_s *req)
 *
 * \brief   Serequest initialize
 *
 * \param   req A variable-length parameters list containing request.
 *
 * \return  An int.
 */

int serequest_init(struct serequest_params_s *req)
{
    req->firstcmd = NULL;
    req->firstrsp = NULL;
    return 0;
}

/*!
 * \fn  int serequest_addapdu(struct serequest_params_s *req, struct apdu_command_s *cmd, struct apdu_response_s *resp)
 *
 * \brief   Chain APDUs into the request list
 *
 * \param       req     the request.
 * \param [in]  cmd     the command.
 * \param [in]  resp    the resp.
 *
 * \return  An int.
 */

int serequest_addapdu(struct serequest_params_s *req, struct apdu_command_s *cmd, struct apdu_response_s *resp)
{
    int ret;
    struct apdu_command_s *c  = req->firstcmd;
    struct apdu_response_s *r = req->firstrsp;
    if (req != NULL && cmd != NULL && resp != NULL)
    {
        if (c != NULL)
        {
            while (c->next != NULL)
            {
                /* TODO we assume here that cmd and resp are synced */
                c = c->next;
                r = r->next;
            };
            c->next = cmd;
            r->next = resp;
        }
        else
        {
            req->firstcmd = cmd;
            req->firstrsp = resp;
        }
        ret = KPL_SUCCESS;
    }
    else
    {
        ret = KPL_ERROR_NULL_POINTER;
    }
    return ret;
}

/*!
 * \fn  int serequest_execute(struct keyple_context_s *ctx, struct reader_s *rd, struct serequest_params_s *req)
 *
 * \brief   Execute all APDUs in a list using plugins
 *
 * \param [in,out]  ctx the context.
 * \param [in,out]  rd  the reader.
 * \param           req A variable-length parameters list containing request.
 *
 * \return  An int.
 */

int serequest_execute(struct keyple_context_s *ctx, struct reader_s *rd, struct serequest_params_s *req)
{
    int ret;
    if (ctx != NULL && rd != NULL && req != NULL)
    {
        struct apdu_command_s *c  = req->firstcmd;
        struct apdu_response_s *r = req->firstrsp;

        while (c != NULL)
        {
            r->len = rd->ops->transmit(rd, c->data, c->len, r->data, r->maxlen);
            c      = c->next;
            r      = r->next;
        }

        ret = KPL_SUCCESS;
    }
    else
    {
        ret = KPL_ERROR_NULL_POINTER;
    }

    return ret;
}
