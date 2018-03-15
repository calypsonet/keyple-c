#ifndef KEYPLE_LOG_H
#define KEYPLE_LOG_H

#ifdef KPL_LOG_ENABLED

// When KPL_LOG_ENABLED is defined, these functions have to be implemented in the hosting environment
void logHexDump(uint8_t *buffer, uint16_t size, bool space);
void logString(char *string, bool newline);

#ifdef KPL_LOG_COMPILER_GIVES_FULL_PATH
// trick to handle full paths if needed
char *logGetBaseName(char *path);
#define __FILENAME__ logGetBaseName(__FILE__)
#else
#define __FILENAME__ __FILE__
#endif // KPL_LOG_COMPILER_GIVES_FULL_PATH

// Macros used by the SDK

#define _LOG_MSG(MSG, NEWLINE)                                                                               \
    do                                                                                                       \
    {                                                                                                        \
        logString(__FILENAME__, false);                                                                      \
        logString("::", false);                                                                              \
        logString(__FUNCTION__, false);                                                                      \
        logString(": ", false);                                                                              \
        logString(MSG, NEWLINE);                                                                             \
    } while (0)

#define LOG_MSG(MSG) _LOG_MSG(MSG, true)

#define LOG_BUFFER(BUF, BUFLEN)                                                                              \
    do                                                                                                       \
    {                                                                                                        \
        logHexDump(BUF, BUFLEN, KPL_LOG_SPACES_IN_HEXDUMPS);                                                 \
    } while (0)

#define LOG_MSG_BUFFER(MSG, BUF, BUFLEN)                                                                     \
    do                                                                                                       \
    {                                                                                                        \
        _LOG_MSG(MSG, false);                                                                                \
        logString(": ", false);                                                                              \
        logHexDump(BUF, BUFLEN, KPL_LOG_SPACES_IN_HEXDUMPS);                                                 \
    } while (0)

#else
#define LOG_MSG(MSG)
#define LOG_BUFFER(MSG)
#define LOG_MSG_BUFFER(MSG)
#endif // KPL_LOG_ENABLED

#endif // KEYPLE_LOG_H
