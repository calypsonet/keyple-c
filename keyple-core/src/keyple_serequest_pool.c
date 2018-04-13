/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

/*!
 * \file    keyple_serequest_pool.c.
 *
 * \brief   Keyple serequest memory management functions
 */

#include <string.h>

#include "keyple.h"
#include "keyple_serequest.h"
#include "keyple_serequest_pool.h"

#define POOL_AVAILABLE_SPACE(p) (pool->size - (pool->next_free - pool->start))

/*!
 * \fn  int serequest_define_pool(struct apdupool_s *pool, uint8_t *pooldata, uint16_t poolsize)
 *
 * \brief   Initialize a pool structure
 *
 * \param [in,out]  pool        the pool.
 * \param [in]      pooldata    the pool buffer allocated by caller.
 * \param           poolsize    The pool size.
 *
 * \return  An int.
 */

int serequest_define_pool(struct apdupool_s *pool, uint8_t *pooldata, uint16_t poolsize)
{
    pool->start = pool->next_free = pooldata;
    pool->size                    = poolsize;
    return 0;
}

/*!
 * \fn  int serequest_reset_pool(struct apdupool_s *pool)
 *
 * \brief   Serequest reset pool
 *
 * \param [in,out]  pool    the pool.
 *
 * \return  An int.
 */

int serequest_reset_pool(struct apdupool_s *pool)
{
    pool->next_free = pool->start;
    return 0;
}

/*!
 * \fn  struct apdu_command_s *serequest_alloccommand(struct apdupool_s *pool, uint16_t data_size)
 *
 * \brief   Allocates a command apdu struct from pool defined with serequest_definepools
 *
 * \param [in]  pool        the pool.
 * \param       data_size   Size of the in data.
 *
 * \return  Null if it fails, else a pointer to the apdu_command_s.
 */

struct apdu_command_s *serequest_alloccommand(struct apdupool_s *pool, uint16_t data_size)
{
    struct apdu_command_s *cmd = NULL;
    if (POOL_AVAILABLE_SPACE(pool) >= sizeof(struct apdu_command_s) + data_size)
    {
        cmd                     = (struct apdu_command_s *)(pool->next_free);
        cmd->data               = pool->next_free + sizeof(struct apdu_command_s);
        cmd->force_get_response = CONFIG_KEYPLE_FORCE_GET_RESPONSE;
        cmd->len                = 0;
        cmd->next               = NULL;
        pool->next_free += sizeof(struct apdu_command_s) + data_size;
    }
    return cmd;
}

/*!
 * \fn  struct apdu_command_s *serequest_alloccommand_ptr(struct apdupool_s *pool, uint16_t data_size, uint8_t *ptr)
 *
 * \brief   Allocates an apdu struct and initialize data pointer to user provided buffer
 *
 * \param [in,out]  pool        the pool.
 * \param           data_size   Size of the in data.
 * \param [in,out]  ptr         the caller pointer to the allocated memory.
 *
 * \return  Null if it fails, else a pointer to the apdu_command_s.
 */

struct apdu_command_s *serequest_alloccommand_ptr(struct apdupool_s *pool, uint16_t data_size, uint8_t *ptr)
{
    struct apdu_command_s *cmd = NULL;
    if (POOL_AVAILABLE_SPACE(pool) >= sizeof(struct apdu_command_s))
    {
        cmd                     = (struct apdu_command_s *)(pool->next_free);
        cmd->data               = ptr;
        cmd->force_get_response = CONFIG_KEYPLE_FORCE_GET_RESPONSE;
        cmd->len                = 0;
        cmd->next               = NULL;
        pool->next_free += sizeof(struct apdu_command_s);
    }
    return cmd;
}

/*!
 * \fn  struct apdu_command_s *serequest_alloccommand_copy(struct apdupool_s *pool, uint16_t data_size, uint8_t *ptr)
 *
 * \brief   Allocates an apdu struct AND space for data, then COPY data from user provided buffer
 *
 * \param [in,out]  pool        the pool.
 * \param           data_size   Size of the in data.
 * \param [in]      ptr         the data to be copied.
 *
 * \return  Null if it fails, else a pointer to the apdu_command_s.
 */

struct apdu_command_s *serequest_alloccommand_copy(struct apdupool_s *pool, uint16_t data_size, uint8_t *ptr)
{
    struct apdu_command_s *cmd = NULL;
    if (POOL_AVAILABLE_SPACE(pool) >= sizeof(struct apdu_command_s) + data_size)
    {
        cmd       = (struct apdu_command_s *)(pool->next_free);
        cmd->data = pool->next_free + sizeof(struct apdu_command_s);
        memcpy(cmd->data, ptr, data_size);
        cmd->force_get_response = CONFIG_KEYPLE_FORCE_GET_RESPONSE;
        cmd->len                = 0;
        cmd->next               = NULL;
        pool->next_free += sizeof(struct apdu_command_s) + data_size;
    }
    return cmd;
}

/*!
 * \fn  struct apdu_command_s * serequest_alloccommand_full(struct apdupool_s *pool, uint8_t *header, uint8_t *data, uint16_t datalen)
 *
 * \brief   Allocates an apdu struct AND space for data, then COPY header AND data from user
 *          provided buffer
 *           serequest_alloccommanddata({CLA,INS,P1,P2,P3}, apdudata, apdudatalen)
 *
 * \param [in]  pool    the pool. 
 * \param [in]  header  the header.
 * \param [in]  data    the data.
 * \param       datalen The datalen.
 *
 * \return  Null if it fails, else a pointer to the apdu_command_s.
 */

struct apdu_command_s *
    serequest_alloccommand_full(struct apdupool_s *pool, uint8_t *header, uint8_t *data, uint16_t datalen)
{
    // not yet implemented
    return NULL;
}

/*!
 * \fn  struct apdu_response_s *serequest_allocresponse(struct apdupool_s *pool, uint16_t data_size)
 *
 * \brief   Allocates a response apdu struct from pool defined with serequest_definepools
 *
 * \param [in]  pool        the pool.
 * \param       data_size   Size of the in data.
 *
 * \return  Null if it fails, else a pointer to the apdu_response_s.
 */

struct apdu_response_s *serequest_allocresponse(struct apdupool_s *pool, uint16_t data_size)
{
    struct apdu_response_s *resp = NULL;
    if (POOL_AVAILABLE_SPACE(pool) >= sizeof(struct apdu_response_s) + data_size)
    {
        resp         = (struct apdu_response_s *)(pool->next_free);
        resp->data   = pool->next_free + sizeof(struct apdu_response_s);
        resp->len    = 0;
        resp->maxlen = data_size;
        resp->next   = NULL;
        pool->next_free += sizeof(struct apdu_command_s) + data_size;
    }
    return resp;
}
