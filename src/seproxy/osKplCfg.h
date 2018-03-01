/**
Summary
*/
#ifndef OS_KPL_CFG_H
#define OS_KPL_CFG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* -------------------------------------------------------------------------- *
 * - Definition of the OS used ---------------------------------------------- *
 * -------------------------------------------------------------------------- */

#define	K_OS_KPL_PLATFORM_WIN32												1

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#if K_OS_KPL_PLATFORM == K_OS_KPL_PLATFORM_WIN32
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <stdint.h>
#endif

/* -------------------------------------------------------------------------- *
 * - Defines ---------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#if K_OS_KPL_PLATFORM == K_OS_KPL_PLATFORM_WIN32

#define K_OS_KPL_DEV_NAME_MAX_LENGTH										256

// Defines the file path buffer length. (Include de character '\0')
#define K_OS_KPL_FILE_PATH_MAX_SIZE											(MAX_PATH + 1)

#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //OS_KPL_CFG_H