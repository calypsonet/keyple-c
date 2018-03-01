/**
Summary
*/

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#include "seProxyMngr.h"
#include "pluginMngr.h"
#include "plugin.h"
#include "osKplMem.h"

#ifdef K_KPL_PCSC_PLUGIN_SUPPORT
#include "pcscPlgn.h"
#endif

/* -------------------------------------------------------------------------- * 
 * - Internal Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

static ExecutionStatus initializePluginTable(void **inOutProxyContext)
{
	T_SeProxyContext *proxyContext = (T_SeProxyContext*) *inOutProxyContext;

	T_PluginEntry localPluginsTable[] = {

		#ifdef K_KPL_PCSC_PLUGIN_SUPPORT
		{K_PLUGIN_TYPE_PCSC, PCSC_FillFunctionTable},
		#endif

		{K_PLUGIN_TYPE_UKNOWN, NULL}
	};

	proxyContext->fPluginTable = (T_PluginEntry*) OsKpl_Malloc(sizeof(localPluginsTable));

	if(proxyContext == NULL) {
		return OUT_OF_MEMORY_ERROR;
	}

	OsKpl_Memcpy(proxyContext->fPluginTable, localPluginsTable, sizeof(localPluginsTable));

	return EXECUTION_OK;
}


static void removePluginFromList(T_SeProxyContext *proxyContext) 
{
	T_PluginContext *currPlugin = proxyContext->fPluginList;

	if(proxyContext->fPluginList == NULL) {
		return;
	}

	proxyContext->fPluginList = currPlugin->fNextPlugin;

	terminatePluginManager(currPlugin);
	OsKpl_Free(currPlugin);
}


/* -------------------------------------------------------------------------- * 
 * - External Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

ExecutionStatus initProxyManager(T_DebugMode inDebugMode, int8_t *inLogPath, uint16_t *outVersion, void **outProxyContext) 
{
	T_SeProxyContext *proxyContext = NULL;
	ExecutionStatus status = EXECUTION_OK;

	if(outVersion == NULL) {return INVALID_PARAMETER_ERROR;}

	proxyContext = (T_SeProxyContext*) OsKpl_Malloc(sizeof(T_SeProxyContext));

	if(proxyContext == NULL) {
		return OUT_OF_MEMORY_ERROR;
	}

	OsKpl_Memset(proxyContext, 0, sizeof(T_SeProxyContext));

	status = initializePluginTable((void **) &proxyContext);

	if(status != EXECUTION_OK) {
		OsKpl_Free(proxyContext);
		return status;
	}

	*outVersion = 1;
	*outProxyContext = (void**) proxyContext;

	return EXECUTION_OK;
}


ExecutionStatus getSupportedPlugins(void *inProxyContext, T_PluginType *outSupportedPlugins, uint16_t *outNumberOfSupportedPlugins)
{
	T_SeProxyContext *proxyContext = (T_SeProxyContext*) inProxyContext;
	uint16_t i;

	if(inProxyContext == NULL) {return INVALID_PARAMETER_ERROR;}
	if(outNumberOfSupportedPlugins == NULL) {return INVALID_PARAMETER_ERROR;}

	for(i = 0; proxyContext->fPluginTable[i].fPluginType != K_PLUGIN_TYPE_UKNOWN; i++) {

		if(outSupportedPlugins != NULL) {
			outSupportedPlugins[i] = proxyContext->fPluginTable[i].fPluginType;
		}

	}

	*outNumberOfSupportedPlugins = i;

	return EXECUTION_OK;
}


ExecutionStatus getPlugin(void *inProxyContext, T_PluginType inPluginType, void **outPluginContext)
{
	T_SeProxyContext *proxyContext = (T_SeProxyContext*) inProxyContext;
	int16_t i;

	if(inProxyContext == NULL) {return INVALID_PARAMETER_ERROR;}

	// Check to see if the plugin is already initialized and we can simply return it.
	if(proxyContext->fPluginList != NULL) {

		T_PluginContext *currentPlugin = proxyContext->fPluginList;

		if(currentPlugin->fPluginType == inPluginType) {
			(T_PluginContext*) *outPluginContext = currentPlugin;
			return EXECUTION_OK;
		} 

		while(currentPlugin->fNextPlugin != NULL) {

			currentPlugin = currentPlugin->fNextPlugin;

			if(currentPlugin->fPluginType == inPluginType) {
				(T_PluginContext*) *outPluginContext = currentPlugin;
				return EXECUTION_OK;
			} 

		}
	}

	// Check to see if the plugin is supported and if it is initialize it, add it to the internal list, and return it.
	for(i = 0; proxyContext->fPluginTable[i].fPluginType != K_PLUGIN_TYPE_UKNOWN; i++) {

		if(proxyContext->fPluginTable[i].fPluginType == inPluginType) {

			T_PluginContext *newPlugin = NULL;
			ExecutionStatus status;

			newPlugin = (T_PluginContext*) OsKpl_Malloc(sizeof(T_PluginContext));

			if(newPlugin == NULL) {
				return OUT_OF_MEMORY_ERROR;
			}

			OsKpl_Memset(newPlugin, 0, sizeof(T_PluginContext));

			status = initPluginManager(proxyContext, inPluginType, newPlugin);

			if(status != EXECUTION_OK) {

				OsKpl_Free(newPlugin);
				return status;

			} else {

				T_PluginContext *currentPlugin = proxyContext->fPluginList;

				if(currentPlugin == NULL) {

					proxyContext->fPluginList = newPlugin;
					(T_PluginContext*) *outPluginContext = newPlugin;
					return EXECUTION_OK;

				} else {

					while(currentPlugin->fNextPlugin != NULL) {
						currentPlugin = currentPlugin->fNextPlugin;
					}

					currentPlugin->fNextPlugin = newPlugin;
					(T_PluginContext*) *outPluginContext = newPlugin;
					return EXECUTION_OK;
				}
			}
			
		}
	}

	return PLUGIN_NOT_SUPPORTED_ERROR;
}



ExecutionStatus terminateProxyManager(void *inProxyContext)
{
	T_SeProxyContext *proxyContext = (T_SeProxyContext*) inProxyContext;

	if(inProxyContext == NULL) {return INVALID_PARAMETER_ERROR;}

	while(proxyContext->fPluginList != NULL) {

		removePluginFromList(proxyContext);
	}

	OsKpl_Free(proxyContext->fPluginTable);

	OsKpl_Free(inProxyContext);
	inProxyContext = NULL;

	return EXECUTION_OK;
}
