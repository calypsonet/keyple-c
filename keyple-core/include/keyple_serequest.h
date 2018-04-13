/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#ifndef SEREQUEST_H
#define SEREQUEST_H

#include <stdint.h>

#define AID_MAX_LENGTH 16
#define GET_SW(rapdu) (((uint16_t)((rapdu)->data[(rapdu)->len - 2]) << 8) | ((rapdu)->data[(rapdu)->len - 1]))

/*!
 * \struct  serequest_params_s
 *
 * \brief   Holds the request parameters such as AID to select, keep channel open flag and apdus linked list.
 */

struct serequest_params_s
{
    uint16_t aidlen;
    uint8_t aid[AID_MAX_LENGTH];
    bool keepChannelOpen;
    struct apdu_command_s *firstcmd;
    struct apdu_response_s *firstrsp;
};

/*!
 * \struct apdu_command_s
 *
 * \brief Holds the information needed to describe a command APDU.
 */

struct apdu_command_s
{
    struct apdu_command_s *next;
    uint16_t len;
    uint8_t *data;
    bool force_get_response;
};

/*!
 * \struct  apdu_response_s
 *
 * \brief   Holds the data received in response to a command APDU.
 */

struct apdu_response_s
{
    struct apdu_response_s *next;
    uint16_t maxlen;
    uint16_t len;
    uint8_t *data;
};

int serequest_init(struct serequest_params_s *req);
int serequest_addapdu(struct serequest_params_s *req, struct apdu_command_s *cmd, struct apdu_response_s *resp);
int serequest_execute(struct keyple_context_s *ctx, struct reader_s *rd, struct serequest_params_s *req);

#endif
