/**
Summary
*/

#ifdef K_KPL_PCSC_PLUGIN_SUPPORT
/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#include "pluginMngr.h"
#include "plugin.h"
#include "osKplMem.h"
#include "osKplStr.h"
#include <winscard.h>

/* -------------------------------------------------------------------------- * 
 * - Defines ---------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#define K_PCSC_NUMBER_OF_PARAMETERS											1

/* -------------------------------------------------------------------------- * 
 * - Type Definitions ------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

typedef struct {

	SCARDCONTEXT hContext;

} T_PCSCPluginContext;


typedef struct {

	SCARDCONTEXT hContext;

	uint32_t fConnectionProtocol;

	// Configurable Parameters
	uint32_t fShareMode;

} T_PCSCReaderContext;

/* -------------------------------------------------------------------------- * 
 * - External Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

ExecutionStatus PCSC_initPluginManager(void* inPluginContext)
{
	T_PCSCPluginContext *pcscData = NULL;
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	uint32_t returnValue;

	pcscData = (T_PCSCPluginContext*) OsKpl_Malloc(sizeof(T_PCSCPluginContext));

	if(pcscData == NULL) {
		return OUT_OF_MEMORY_ERROR;
	}

	OsKpl_Memset(pcscData, 0, sizeof(T_PCSCPluginContext));

	returnValue = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &(pcscData->hContext));

	if(returnValue != SCARD_S_SUCCESS) {
		OsKpl_Free(pcscData);
		return PLUGIN_INITIALIZATION_ERROR;
	}


	(T_PCSCPluginContext*) pluginContext->fPluginInternalData = pcscData;

	return EXECUTION_OK;
}


ExecutionStatus PCSC_getPluginName(int8_t *outPluginName)
{
	OsKpl_Strcpy(outPluginName, "PC/SC");

	return EXECUTION_OK;
}


ExecutionStatus PCSC_getReaderDevNames(void *inPluginContext, int8_t *outReaderDevNames, uint32_t *outReaderDevNamesSize)
{
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	T_PCSCPluginContext *pcscData = (T_PCSCPluginContext*) pluginContext->fPluginInternalData;
	uint32_t returnValue;

	returnValue = SCardListReaders(pcscData->hContext, NULL, outReaderDevNames, outReaderDevNamesSize);

	if(returnValue != SCARD_S_SUCCESS) {
		return PLUGIN_SPECIFIC_ERROR;
	}

	return EXECUTION_OK;
}


ExecutionStatus PCSC_getReaderByDevName(void *inPluginContext, int8_t *inReaderDevName, void *outProxyReader)
{
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	T_PCSCPluginContext *pcscData = (T_PCSCPluginContext*) pluginContext->fPluginInternalData;
	T_ProxyReader *proxyReader = (T_ProxyReader*) outProxyReader;
	int8_t *readerDevNames = NULL;
	uint32_t readerDevNamesSize, returnValue, iterReaders;

	returnValue = SCardListReaders(pcscData->hContext, NULL, readerDevNames, &readerDevNamesSize);

	if(returnValue != SCARD_S_SUCCESS) {
		return PLUGIN_SPECIFIC_ERROR;
	}

	readerDevNames = (int8_t*) OsKpl_Malloc(readerDevNamesSize);

	if(readerDevNames == NULL) {
		return OUT_OF_MEMORY_ERROR;
	}

	returnValue = SCardListReaders(pcscData->hContext, NULL, readerDevNames, &readerDevNamesSize);

	if(returnValue != SCARD_S_SUCCESS) {
		OsKpl_Free(readerDevNames);
		return PLUGIN_SPECIFIC_ERROR;
	}

	for(iterReaders = 0; iterReaders < readerDevNamesSize; iterReaders++) {

		if(!OsKpl_Strcmp(readerDevNames+iterReaders, inReaderDevName))
		{
			T_PCSCReaderContext *pcscReaderData = NULL;
			OsKpl_Free(readerDevNames);

			pcscReaderData = (T_PCSCReaderContext*) OsKpl_Malloc(sizeof(T_PCSCReaderContext));

			if(pcscReaderData == NULL) {
				return OUT_OF_MEMORY_ERROR;
			}

			OsKpl_Memset(pcscReaderData, 0, sizeof(T_PCSCReaderContext));

			pcscReaderData->hContext = pcscData->hContext;
			pcscReaderData->fShareMode = SCARD_SHARE_SHARED;
			
			proxyReader->fReaderInternalData = pcscReaderData;

			// Set any specific reader information here.

			return EXECUTION_OK;
		}
		iterReaders += OsKpl_Strlen(readerDevNames+iterReaders);
	}

	OsKpl_Free(readerDevNames);

	return UNKNOWN_PROXY_READER_NAME_ERROR;
}


ExecutionStatus PCSC_setParameter(void *inProxyReader, T_ProxyReaderParameter *inParameter)
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	T_PCSCReaderContext *pcscReaderData = proxyReader->fReaderInternalData;

	if(!OsKpl_Strcmp(inParameter->fParameterName, "share_mode")) {

		if(!OsKpl_Strcmp(inParameter->fParameterValue, "shared")) {
			pcscReaderData->fShareMode = SCARD_SHARE_SHARED;
		} else if(!OsKpl_Strcmp(inParameter->fParameterValue, "exclusive")) {
			pcscReaderData->fShareMode = SCARD_SHARE_EXCLUSIVE;
		} else {
			return INCONSISTENT_PARAMETER_VALUE_ERROR;
		}

	} else {
		return WRONG_PARAMETER_KEY_ERROR;
	}

	return EXECUTION_OK;
}


ExecutionStatus PCSC_getParameters(void *inProxyReader, T_ProxyReaderParameter *outParameters, uint32_t *outNumberOfParameters) 
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	T_PCSCReaderContext *pcscReaderData = proxyReader->fReaderInternalData;

	*outNumberOfParameters = K_PCSC_NUMBER_OF_PARAMETERS;

	if(outParameters == NULL) {
		return EXECUTION_OK;
	}

	OsKpl_Strcpy(outParameters[0].fParameterName, "share_mode");

	if(pcscReaderData->fShareMode == SCARD_SHARE_SHARED) {
		OsKpl_Strcpy(outParameters[0].fParameterValue, "shared");
	} else if(pcscReaderData->fShareMode == SCARD_SHARE_EXCLUSIVE){
		OsKpl_Strcpy(outParameters[0].fParameterValue, "exclusive");
	} else {
		OsKpl_Strcpy(outParameters[0].fParameterValue, "-");
	}

	return EXECUTION_OK;
}


ExecutionStatus PCSC_getSeStatus(void *inProxyReader)
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	T_PCSCReaderContext *pcscReaderData = proxyReader->fReaderInternalData;
	uint32_t returnValue;

	if(proxyReader->fConnectionState != K_READER_CONNECTION_STATE_CONNECTED) {

		returnValue = SCardConnect(
			pcscReaderData->hContext, 
			proxyReader->fDeviceName, 
			pcscReaderData->fShareMode, 
			SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
			&(proxyReader->fInternalHandler),
			&(pcscReaderData->fConnectionProtocol));

		if((returnValue == SCARD_E_NO_SMARTCARD) || (returnValue == SCARD_W_REMOVED_CARD))
		{
			proxyReader->fInternalHandler = -1;
			proxyReader->fConnectionState = K_READER_CONNECTION_STATE_NOT_CONNECTED_YET;

			return SE_NOT_FOUND;
		}
		else if(returnValue != SCARD_S_SUCCESS)
		{
			if(returnValue == SCARD_E_UNKNOWN_READER)
			{
				return PLUGIN_SPECIFIC_ERROR;
			}
			else
			{
				SCardDisconnect(proxyReader->fInternalHandler, SCARD_UNPOWER_CARD);
				return PLUGIN_SPECIFIC_ERROR;
			}
		}
		
		proxyReader->fConnectionState = K_READER_CONNECTION_STATE_CONNECTED;
	}

	if(proxyReader->fConnectionState == K_READER_CONNECTION_STATE_CONNECTED)
	{
		int8_t readerNames[200];
		uint32_t readerNamesLength = 200;
		uint32_t dwState;

		proxyReader->fSeAtrLength = K_ATR_MAX_LENGTH;

		returnValue = SCardStatus(
			proxyReader->fInternalHandler,
			readerNames,
			&readerNamesLength,
			&dwState,
			&(pcscReaderData->fConnectionProtocol),
			proxyReader->fSeAtr,
			(uint32_t*) &(proxyReader->fSeAtrLength));

		if( returnValue != SCARD_S_SUCCESS )
		{
			proxyReader->fConnectionState = K_READER_CONNECTION_STATE_NOT_CONNECTED_YET;
			return SE_NOT_FOUND;
		}

	}

	return EXECUTION_OK;
}


ExecutionStatus PCSC_transmit(void *inProxyReader, uint8_t *inCommand, uint16_t inCommandLength, uint8_t *outResponse, uint16_t *outResponseLength)
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	T_PCSCReaderContext *pcscReaderData = proxyReader->fReaderInternalData;
	uint32_t returnValue; 
	uint32_t responseApduLength = *outResponseLength;

	if(pcscReaderData->fConnectionProtocol == SCARD_PROTOCOL_T1) {

		returnValue = SCardTransmit(
			proxyReader->fInternalHandler,
			SCARD_PCI_T1, 
			inCommand,
			(uint32_t)inCommandLength,
			NULL,
			outResponse,
			&responseApduLength);
		
	} else {
		
		returnValue = SCardTransmit(
			proxyReader->fInternalHandler,
			SCARD_PCI_T0, 
			inCommand,
			(uint32_t)inCommandLength,
			NULL,
			outResponse,
			&responseApduLength);
	}

	if(returnValue != SCARD_S_SUCCESS) {
		return IO_READER_ERROR;	
	}

	*outResponseLength = (uint16_t) (responseApduLength & 0xFFFF);

	return EXECUTION_OK;
}


ExecutionStatus PCSC_closeChannel(void *inProxyReader) 
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;

	SCardDisconnect(proxyReader->fInternalHandler, SCARD_UNPOWER_CARD);
	proxyReader->fConnectionState = K_READER_CONNECTION_STATE_DISCONNECTED;

	return EXECUTION_OK;
}


ExecutionStatus PCSC_terminateProxyReader(void *inProxyReader)
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;

	if(proxyReader->fConnectionState == K_READER_CONNECTION_STATE_CONNECTED) {
		SCardDisconnect(proxyReader->fInternalHandler, SCARD_UNPOWER_CARD);
	}

	OsKpl_Free(proxyReader->fReaderInternalData);

	return EXECUTION_OK;
}


ExecutionStatus PCSC_terminatePluginManager(void* inPluginContext) 
{
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	T_PCSCPluginContext *pcscData = (T_PCSCPluginContext*) pluginContext->fPluginInternalData;
	uint32_t returnValue;

	returnValue = SCardReleaseContext(pcscData->hContext);

	OsKpl_Free(pcscData);

	if(returnValue != SCARD_S_SUCCESS) {
		return PLUGIN_SPECIFIC_ERROR;
	}

	return EXECUTION_OK;
}


void PCSC_FillFunctionTable(void *outFunctionTable)
{
	T_ProxyReaderFT *functionTable = (T_ProxyReaderFT*) outFunctionTable;

	if(outFunctionTable == NULL) {
		return;
	}

	OsKpl_Memset(functionTable, 0, sizeof(T_ProxyReaderFT));

	functionTable->initPluginManager = PCSC_initPluginManager;
	functionTable->getPluginName = PCSC_getPluginName;
	functionTable->getReaderDevNames = PCSC_getReaderDevNames;
	functionTable->getReaderByDevName = PCSC_getReaderByDevName;
	functionTable->setParameter = PCSC_setParameter;
	functionTable->getParameters = PCSC_getParameters;
	functionTable->getSeStatus = PCSC_getSeStatus;
	functionTable->transmit = PCSC_transmit;
	functionTable->closeChannel = PCSC_closeChannel;
	functionTable->terminateProxyReader = PCSC_terminateProxyReader;
	functionTable->terminatePluginManager = PCSC_terminatePluginManager;
}

#endif //K_KPL_PCSC_PLUGIN_SUPPORT