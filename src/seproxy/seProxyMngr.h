/**
Summary
*/
#ifndef SE_PROXY_MNGR_H
#define SE_PROXY_MNGR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#include "seProxy.h"

/* -------------------------------------------------------------------------- * 
 * - External Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

/**
Description
...

@param
inDebugMode - SE Proxy Manager debug mode.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- OUT_OF_MEMORY_ERROR

@see
T_DebugMode
@see
ExecutionStatus
*/
ExecutionStatus initProxyManager(T_DebugMode inDebugMode, int8_t *inLogPath, uint16_t *outVersion, void **outProxyContext);


/**
Description
...

@param
inProxyContext - Pointer to the SE Proxy Manager Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR

@see
T_PluginType
@see
ExecutionStatus
*/
ExecutionStatus getSupportedPlugins(void *inProxyContext, T_PluginType *outSupportedPlugins, uint16_t *outNumberOfSupportedPlugins);


/**
Description
...

@param
inProxyContext - Pointer to the SE Proxy Manager Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- OUT_OF_MEMORY_ERROR
- PLUGIN_NOT_SUPPORTED_ERROR
- PLUGIN_INITIALIZATION_ERROR

@see
T_PluginType
@see
ExecutionStatus
*/
ExecutionStatus getPlugin(void *inProxyContext, T_PluginType inPluginType, void **outPluginContext);


/**
Description
...

@param
inPluginContext - Pointer to the Plugin Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- FUNCTION_TABLE_NOT_INITIALIZED_ERROR

@see
ExecutionStatus
*/
ExecutionStatus getPluginName(void *inPluginContext, int8_t *outPluginName);


/**
Description
...

@param
inPluginContext - Pointer to the Plugin Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- FUNCTION_TABLE_NOT_INITIALIZED_ERROR
- PLUGIN_SPECIFIC_ERROR

@see
ExecutionStatus
*/
ExecutionStatus getReaderDevNames(void *inPluginContext, int8_t *outReaderDevNames, uint32_t *outReaderDevNamesSize);


/**
Description
...

@param
inPluginContext - Pointer to the Plugin Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- OUT_OF_MEMORY_ERROR
- FUNCTION_TABLE_NOT_INITIALIZED_ERROR
- PLUGIN_SPECIFIC_ERROR
- UNKNOWN_PROXY_READER_NAME_ERROR

@see
ExecutionStatus
*/
ExecutionStatus getReaderByDevName(void *inPluginContext, int8_t *inReaderDevName, void **outProxyReader);


/**
Description
...

@param
inProxyReader - Pointer to the Reader Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- FUNCTION_TABLE_NOT_INITIALIZED_ERROR
- INCONSISTENT_PARAMETER_VALUE_ERROR
- WRONG_PARAMETER_KEY_ERROR

@see
ExecutionStatus
*/
ExecutionStatus setReaderParameters(void *inProxyReader, T_ProxyReaderParameter *inParameters, uint32_t inNumberOfParameters);


/**
Description
...

@param
inProxyReader - Pointer to the Reader Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- FUNCTION_TABLE_NOT_INITIALIZED_ERROR

@see
ExecutionStatus
*/
ExecutionStatus getReaderParameters(void *inProxyReader, T_ProxyReaderParameter *outParameters, uint32_t *outNumberOfParameters);


/**
Description
...

@param
inProxyReader - Pointer to the Reader Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- FUNCTION_TABLE_NOT_INITIALIZED_ERROR
- PLUGIN_SPECIFIC_ERROR
- SE_NOT_FOUND

@see
ExecutionStatus
*/
ExecutionStatus getSeStatus(void *inProxyReader);


/**
Description
...

@param
inProxyReader - Pointer to the Reader Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR
- FUNCTION_TABLE_NOT_INITIALIZED_ERROR
- IO_READER_ERROR

@see
ExecutionStatus
*/
ExecutionStatus transmitRequests(void *inProxyReader, T_SeRequest *inRequests, T_SeResponse *outResponses);


/**
Description
...

@param
inProxyContext - Pointer to the SE Proxy Manager Context.

@return
ExecutionStatus:
- EXECUTION_OK
- INVALID_PARAMETER_ERROR

@see
ExecutionStatus
*/
ExecutionStatus terminateProxyManager(void *inProxyContext);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //SE_PROXY_MNGR_H