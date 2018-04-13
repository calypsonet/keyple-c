/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#include "keyple.h"

struct keyple_context_s keyple_ctx;

/*!
 * \fn  int keyple_init(struct keyple_context_s *ctx)
 *
 * \brief   Keyple middleware global initialization
 *
 * \param [in,out]  ctx the keyple context.
 *
 * \return  the operation status.
 */

int keyple_init(struct keyple_context_s *ctx)
{
    ctx->reader_count = 0;
#ifdef CONFIG_KEYPLE_DYNAMIC_READERS
    ctx->more_readers = NULL;
#endif
#ifdef CONFIG_KEYPLE_LOGS
    ctx->logs = NULL;
#endif
    return 0;
}

/*!
 * \fn  int keyple_addreader(struct keyple_context_s *ctx, struct reader_s *rd)
 *
 * \brief   register a reader into the provided keyple context
 *
 * \param [in,out]  ctx the keyple context.
 * \param [in]      rd  the reader descriptor.
 *
 * \return  the operation status.
 */

int keyple_addreader(struct keyple_context_s *ctx, struct reader_s *rd)
{
    int ret;
    if (ctx->reader_count < CONFIG_KEYPLE_MAX_READERS)
    {
        ctx->readers[ctx->reader_count] = rd;
        ctx->reader_count++;
        ret = KPL_SUCCESS;
    }
    else
    {
        ret = KPL_MAX_ALLOWED_READERS_NUMBER_REACHED;
    }
    return 0;
}

#ifdef CONFIG_KEYPLE_LOGS
int keyple_setlogcallback(struct keyple_context_s *ctx, struct keyple_log_s *logs)
{
    return 0;
}
#endif
