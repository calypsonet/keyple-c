/**
Summary
*/
#ifndef PLUGIN_H
#define PLUGIN_H

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
#include "seProxy.h"

/* -------------------------------------------------------------------------- * 
 * - Defines ---------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#define K_PLUGIN_MANAGER_NUMBER_OF_PARAMETERS								1

/* -------------------------------------------------------------------------- * 
 * - Type Definitions ------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

typedef enum {

	K_READER_CONNECTION_STATE_UNKNOWN = 0,

	K_READER_CONNECTION_STATE_CONNECTED,

	K_READER_CONNECTION_STATE_NOT_CONNECTED_YET,

	K_READER_CONNECTION_STATE_LOST,

	K_READER_CONNECTION_STATE_DISCONNECTED,

	K_READER_CONNECTION_STATE_CONNECTED_DIRECTLY_TO_READER

} T_ReaderConnectionState;


typedef struct {

	ExecutionStatus (*initPluginManager)(void * inOutPluginContext);

	ExecutionStatus (*getPluginName)(int8_t *outPluginName);

	ExecutionStatus (*getReaderDevNames)(void *inPluginContext, int8_t *outReaderDevNames, uint32_t *outReaderDevNamesSize);

	ExecutionStatus (*getReaderByDevName)(void *inPluginContext, int8_t *inReaderDevName, void *outProxyReader);

	ExecutionStatus (*setParameter)(void *inProxyReader, T_ProxyReaderParameter *inParameters);

	ExecutionStatus (*getParameters)(void *inProxyReader, T_ProxyReaderParameter *outParameters, uint32_t *outNumberOfParameters);

	ExecutionStatus (*getSeStatus)(void *inProxyReader);

	ExecutionStatus (*transmit)(void *inProxyReader, uint8_t *inCommand, uint16_t inCommandLength, uint8_t *outResponse, uint16_t *outResponseLength);

	ExecutionStatus (*closeChannel)(void *inProxyReader);

	ExecutionStatus (*terminateProxyReader)(void *inProxyReader);

	ExecutionStatus (*terminatePluginManager)(void *inPluginContext);

} T_ProxyReaderFT;


typedef struct {

	int8_t fDeviceName[K_OS_KPL_DEV_NAME_MAX_LENGTH];

	int32_t fInternalHandler;

	T_ProxyReaderFT *fProxyReaderTable;

	void *fReaderInternalData;

	T_ReaderConnectionState fConnectionState;

	uint8_t fSeAtr[K_ATR_MAX_LENGTH];

	uint16_t fSeAtrLength;

	void *fNextReader;

	// Configurable Parameters
	uint8_t fTransmitBreakOnFailFlag;

} T_ProxyReader;


typedef struct {

	T_PluginType fPluginType;

	T_ProxyReader *fReaderList;

	T_ProxyReaderFT fProxyReaderTable;

	void *fPluginInternalData;

	void *fNextPlugin;

} T_PluginContext;


typedef void (*GetPluginFunctionTable)(void*);


typedef struct {

	T_PluginType fPluginType;

	GetPluginFunctionTable fGetPluginFunctionTable;

} T_PluginEntry;


typedef struct {

	T_PluginContext *fPluginList;

	T_PluginEntry *fPluginTable;

	int8_t fLogPath[K_OS_KPL_FILE_PATH_MAX_SIZE];

} T_SeProxyContext;

#endif //PLUGIN_H