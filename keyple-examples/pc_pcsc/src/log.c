#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "keyple_config.h"
#include "keyple_log.h"

void logHexDump(uint8_t *buffer, uint16_t size, bool space)
{
    char *format = space ? "%02X " : "%02X";
    for (uint16_t ii = 0; ii < size; ii++)
    {
        printf(format, buffer[ii]);
    }
    printf("\n");
}

void logString(char *string, bool newline)
{
    if (newline)
    {
        printf("%s\n", string);
    }
    else
    {
        printf("%s", string);
    }
}

char *logGetBaseName(char *path)
{
    char *basename;
    basename = path + strlen(path);
    for (; basename > path; basename--)
    {
        if ((*basename == '\\') || (*basename == '/'))
        {
            basename++;
            break;
        }
    }
    return basename;
}
