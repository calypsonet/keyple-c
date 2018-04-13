/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#ifndef KEYPLE_H
#define KEYPLE_H

#include <stdint.h>
#include <stdbool.h>
#include "keyple_errors.h"
#include "keyple_config.h"
#include "keyple_reader.h"
#include "keyple_se_identifiers.h"

#define CONFIG_KEYPLE_FORCE_GET_RESPONSE true
#define CONFIG_KEYPLE_LOGS

#ifdef CONFIG_KEYPLE_DYNAMIC_READERS
struct readerlist_s
{
    struct reader_s *reader;
    struct readerlist_s *next;
};
#endif

struct keyple_context_s;

#ifdef CONFIG_KEYPLE_LOGS
struct keyple_log_s
{
    void (*log)(char *function, int linenum, char *fmt, uint32_t val);
    void (*logbuf)(char *function, int linenum, uint8_t *buf, uint32_t len);
};
#endif

struct keyple_context_s
{
    struct reader_s *readers[CONFIG_KEYPLE_MAX_READERS];
    int reader_count;
#ifdef CONFIG_KEYPLE_DYNAMIC_READERS
    struct reader_s *more_readers;
#endif
#ifdef CONFIG_KEYPLE_LOGS
    void *logctx;
    struct keyple_log_s *logs;
#endif
};

#ifdef CONFIG_KEYPLE_LOGS
#define KEYPLE_LOGTXT(ctx, txt) KEYPLE_LOGVAL(ctx, txt, 0)
#define KEYPLE_LOGVAL(ctx, fmt, val)                         \
    do                                                       \
    {                                                        \
        if (ctx.logs)                                        \
        {                                                    \
            ctx.logs->log(__FUNCTION__, __LINE__, fmt, val); \
        }                                                    \
    } while (0)

#define KEYPLE_LOGBUF(ctx, buf, len)                            \
    do                                                          \
    {                                                           \
        if (ctx.logs)                                           \
        {                                                       \
            ctx.logs->logbuf(__FUNCTION__, __LINE__, buf, len); \
        }                                                       \
    } while (0)
#else
#define KEYPLE_LOGTXT(ctx, text)
#define KEYPLE_LOGVAL(ctx, fmt, val)
#define KEYPLE_LOGBUF(ctx, buf, len)
#endif

extern struct keyple_context_s keyple_ctx;

int keyple_init(struct keyple_context_s *ctx);
int keyple_addreader(struct keyple_context_s *ctx, struct reader_s *rd);

#ifdef CONFIG_KEYPLE_LOGS
int keyple_setlogcallback(struct keyple_context_s *ctx, struct keyple_log_s *logs);
#endif

#endif //KEYPLE_H
