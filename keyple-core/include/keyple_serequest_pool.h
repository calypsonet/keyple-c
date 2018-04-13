/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#ifndef SEREQUEST_POOL_H
#define SEREQUEST_POOL_H

#include <stdint.h>

struct apdupool_s
{
    uint8_t *start;
    uint8_t *next_free;
    uint16_t size;
};

int serequest_define_pool(struct apdupool_s *pool, uint8_t *pooldata, int poolsize);
int serequest_reset_pool(struct apdupool_s *pool);
struct apdu_command_s *serequest_alloccommand(struct apdupool_s *pool, uint16_t inDataSize);
struct apdu_command_s *serequest_alloccommand_ptr(struct apdupool_s *pool, uint16_t inDataSize, uint8_t *ptr);
struct apdu_command_s *serequest_alloccommand_copy(struct apdupool_s *pool, uint16_t inDataSize, uint8_t *ptr);
struct apdu_command_s *
    serequest_alloccommand_full(struct apdupool_s *pool, uint8_t *header, uint8_t *data, uint16_t datalen);
struct apdu_response_s *serequest_allocresponse(struct apdupool_s *pool, uint16_t inDataSize);

#endif // SEREQUEST_POOL_H
