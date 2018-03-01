/**
Summary
*/

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#include "pluginMngr.h"
#include "plugin.h"
#include "osKplMem.h"
#include "osKplStr.h"

/* -------------------------------------------------------------------------- * 
 * - Internal Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

static void addReaderToList(T_PluginContext *inPluginContext, T_ProxyReader *inProxyReader)
{
	T_ProxyReader *iterProxyReader = inPluginContext->fReaderList;

	if(inPluginContext->fReaderList == NULL) {
		inPluginContext->fReaderList = inProxyReader;
		return;
	}

	while(iterProxyReader->fNextReader != NULL) {
		iterProxyReader = iterProxyReader->fNextReader;
	}

	iterProxyReader->fNextReader = inProxyReader;
}


static void removeReaderFromList(T_PluginContext *inPluginContext) 
{
	T_ProxyReader *currProxyReader = inPluginContext->fReaderList;

	if(inPluginContext->fReaderList == NULL) {
		return;
	}

	inPluginContext->fReaderList = currProxyReader->fNextReader;

	currProxyReader->fProxyReaderTable->terminateProxyReader(currProxyReader);
	OsKpl_Free(currProxyReader);
}


static ExecutionStatus getPluginTableByType(T_PluginEntry *inPluginTable, T_PluginType inPluginType, T_ProxyReaderFT *outFunctionTable) 
{
	uint16_t i;

	if(inPluginTable == NULL) {
		return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;
	}

	for(i = 0; inPluginTable[i].fPluginType != K_PLUGIN_TYPE_UKNOWN; i++) {

		if(inPluginTable[i].fPluginType == inPluginType) {
			inPluginTable[i].fGetPluginFunctionTable(outFunctionTable);
			return EXECUTION_OK;
		}
	}

	return PLUGIN_NOT_SUPPORTED_ERROR;
}


/* -------------------------------------------------------------------------- * 
 * - Internal Shared Functions ---------------------------------------------- *
 * -------------------------------------------------------------------------- */

ExecutionStatus initPluginManager(void *inProxyContext, T_PluginType inPluginType, void *outPluginContext) 
{
	T_SeProxyContext *proxyContext = (T_SeProxyContext*) inProxyContext;
	T_PluginContext *pluginContext = (T_PluginContext*) outPluginContext;
	ExecutionStatus status;

	if(inProxyContext == NULL) {return INVALID_PARAMETER_ERROR;}
	if(outPluginContext == NULL) {return INVALID_PARAMETER_ERROR;}

	pluginContext->fPluginType = inPluginType;

	status = getPluginTableByType(proxyContext->fPluginTable, inPluginType, &(pluginContext->fProxyReaderTable));
	if(status != EXECUTION_OK) {
		return status;
	}
	
	status = pluginContext->fProxyReaderTable.initPluginManager(pluginContext);
	if(status != EXECUTION_OK) {
		return status;
	}

	return EXECUTION_OK;
}


ExecutionStatus terminatePluginManager(void *inPluginContext)
{
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	ExecutionStatus status;

	while(pluginContext->fReaderList != NULL) {
		removeReaderFromList(pluginContext);
	}

	status = pluginContext->fProxyReaderTable.terminatePluginManager(inPluginContext);

	return status;
}


static ExecutionStatus checkResponse(T_ProxyReader *inProxyReader, uint8_t *inOutResponse, uint16_t *inOutResponseLength)
{
	uint8_t getResponseCmd[] = {0x00, 0xC0, 0x00, 0x00, 0x00};
	uint16_t getResponseCmdLength = 5;
	ExecutionStatus status;

	if((*inOutResponseLength) > 2 || inOutResponse[0] != 0x90 || inOutResponse[1] != 0x00) {
		return EXECUTION_OK;
	}

	*inOutResponseLength = K_APDU_MAX_LENGTH;
	status = inProxyReader->fProxyReaderTable->transmit(
		inProxyReader,
		getResponseCmd,
		getResponseCmdLength,
		inOutResponse,
		inOutResponseLength);

	if(status != EXECUTION_OK) {
		return status;
	}

	if(inOutResponse[0] == 0x61 || inOutResponse[0] == 0x6C) {

		getResponseCmd[4] = inOutResponse[1];

		*inOutResponseLength = K_APDU_MAX_LENGTH;
		status = inProxyReader->fProxyReaderTable->transmit(
			inProxyReader,
			getResponseCmd,
			getResponseCmdLength,
			inOutResponse,
			inOutResponseLength);

		if(status != EXECUTION_OK) {
			return status;
		}
	} 

	return EXECUTION_OK;
}


static ExecutionStatus selectApplication(T_ProxyReader *inProxyReader, uint8_t *inAid, uint16_t inAidLength, uint8_t *outFci, uint16_t *outFciLength)
{
	uint8_t command[K_APDU_MAX_LENGTH];
	uint16_t commandLength = 0;
	uint8_t response[K_APDU_MAX_LENGTH];
	uint16_t responseLength = K_APDU_MAX_LENGTH;
	ExecutionStatus status;

	// Pack Command
	command[commandLength++] = 0x00; //CLA
	command[commandLength++] = 0xA4; //INS
	command[commandLength++] = 0x04; //P1 - Select Application
	command[commandLength++] = 0x04; //P2 - Select first occurrence and return the FCI
	command[commandLength++] = (uint8_t) (inAidLength & 0xFF); //Lc
	OsKpl_Memcpy(command+commandLength, inAid, inAidLength); //DataIn
	commandLength+= inAidLength;


	status = inProxyReader->fProxyReaderTable->transmit(
		inProxyReader,
		command,
		commandLength,
		response,
		&responseLength);

	if(status != EXECUTION_OK) {
		return status;
	}

	status = checkResponse(inProxyReader, response, &responseLength);

	if(status != EXECUTION_OK) {
		return status;
	}

	OsKpl_Memcpy(outFci, response, responseLength);
	*outFciLength = responseLength;

	return EXECUTION_OK;
}


/* -------------------------------------------------------------------------- * 
 * - External Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

ExecutionStatus getPluginName(void *inPluginContext, int8_t *outPluginName)
{
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	ExecutionStatus status;

	if(inPluginContext == NULL) {return INVALID_PARAMETER_ERROR;}
	if(pluginContext->fProxyReaderTable.getPluginName == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(outPluginName == NULL) {return INVALID_PARAMETER_ERROR;}

	status = pluginContext->fProxyReaderTable.getPluginName(outPluginName);

	return status;
}


ExecutionStatus getReaderDevNames(void *inPluginContext, int8_t *outReaderDevNames, uint32_t *outReaderDevNamesSize)
{
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	ExecutionStatus status;

	if(inPluginContext == NULL) {return INVALID_PARAMETER_ERROR;}
	if(pluginContext->fProxyReaderTable.getReaderDevNames == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(outReaderDevNamesSize == NULL) {return INVALID_PARAMETER_ERROR;}

	status = pluginContext->fProxyReaderTable.getReaderDevNames(inPluginContext, outReaderDevNames, outReaderDevNamesSize);

	return status;
}


ExecutionStatus getReaderByDevName(void *inPluginContext, int8_t *inReaderDevName, void **outProxyReader)
{
	T_PluginContext *pluginContext = (T_PluginContext*) inPluginContext;
	T_ProxyReader *proxyReader = NULL;
	ExecutionStatus status = EXECUTION_OK;

	if(inPluginContext == NULL) {return INVALID_PARAMETER_ERROR;}
	if(pluginContext->fProxyReaderTable.getReaderByDevName == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(inReaderDevName == NULL) {return INVALID_PARAMETER_ERROR;}

	proxyReader = (T_ProxyReader*) OsKpl_Malloc(sizeof(T_ProxyReader));

	if(proxyReader == NULL) {
		return OUT_OF_MEMORY_ERROR;
	}

	OsKpl_Memset(proxyReader, 0, sizeof(T_ProxyReader));

	OsKpl_Strcpy(proxyReader->fDeviceName, inReaderDevName);

	status = pluginContext->fProxyReaderTable.getReaderByDevName(inPluginContext, inReaderDevName, proxyReader);

	if(status != EXECUTION_OK) {
		OsKpl_Free(proxyReader);
		return status;
	}

	addReaderToList(pluginContext, proxyReader);

	proxyReader->fProxyReaderTable = &(pluginContext->fProxyReaderTable);

	*outProxyReader = proxyReader;

	return EXECUTION_OK;
}


ExecutionStatus setReaderParameters(void *inProxyReader, T_ProxyReaderParameter *inParameters, uint32_t inNumberOfParameters) 
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	ExecutionStatus status;

	if(inProxyReader == NULL) {return INVALID_PARAMETER_ERROR;}
	if(proxyReader->fProxyReaderTable == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(proxyReader->fProxyReaderTable->setParameter == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(inParameters == NULL) {return INVALID_PARAMETER_ERROR;}

	uint32_t i;

	for(i = 0; i < inNumberOfParameters; i++) {
	
		if(!OsKpl_Strcmp(inParameters[i].fParameterName, "transmit_break_on_fail")) {

			if(!OsKpl_Strcmp(inParameters[i].fParameterValue, "true")) {
				proxyReader->fTransmitBreakOnFailFlag = 1;
			} else if(!OsKpl_Strcmp(inParameters[i].fParameterValue, "false")) {
				proxyReader->fTransmitBreakOnFailFlag = 0;
			} else {
				return INCONSISTENT_PARAMETER_VALUE_ERROR;
			}

		} else {

			status = proxyReader->fProxyReaderTable->setParameter(inProxyReader, &(inParameters[i]));

			if(status != EXECUTION_OK) {
				return status;
			}
		}
	}
	
	return EXECUTION_OK;
}


ExecutionStatus getReaderParameters(void *inProxyReader, T_ProxyReaderParameter *outParameters, uint32_t *outNumberOfParameters) 
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	ExecutionStatus status;
	uint32_t pluginNumberOfParameters;

	if(inProxyReader == NULL) {return INVALID_PARAMETER_ERROR;}
	if(proxyReader->fProxyReaderTable == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(proxyReader->fProxyReaderTable->getParameters == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(outNumberOfParameters == NULL) {return INVALID_PARAMETER_ERROR;}

	status = proxyReader->fProxyReaderTable->getParameters(inProxyReader, outParameters, &pluginNumberOfParameters);

	if(status != EXECUTION_OK) {
		return status;
	}

	*outNumberOfParameters = pluginNumberOfParameters +	K_PLUGIN_MANAGER_NUMBER_OF_PARAMETERS;

	if(outParameters == NULL) {
		return EXECUTION_OK;
	}

	OsKpl_Strcpy(outParameters[pluginNumberOfParameters].fParameterName, "transmit_break_on_fail");

	if(proxyReader->fTransmitBreakOnFailFlag == 0) {
		OsKpl_Strcpy(outParameters[pluginNumberOfParameters].fParameterValue, "false");
	} else if(proxyReader->fTransmitBreakOnFailFlag == 1){
		OsKpl_Strcpy(outParameters[pluginNumberOfParameters].fParameterValue, "true");
	} else {
		OsKpl_Strcpy(outParameters[pluginNumberOfParameters].fParameterValue, "-");
	}

	return EXECUTION_OK;
}


ExecutionStatus getSeStatus(void *inProxyReader)
{
	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	ExecutionStatus status;

	if(inProxyReader == NULL) {return INVALID_PARAMETER_ERROR;}
	if(proxyReader->fProxyReaderTable == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(proxyReader->fProxyReaderTable->getSeStatus == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}

	status = proxyReader->fProxyReaderTable->getSeStatus(inProxyReader);

	return status;
}


ExecutionStatus transmitRequests(void *inProxyReader, T_SeRequest *inRequests, T_SeResponse *outResponses) {

	T_ProxyReader *proxyReader = (T_ProxyReader*) inProxyReader;
	ExecutionStatus status;
	uint32_t i;

	if(inProxyReader == NULL) {return INVALID_PARAMETER_ERROR;}
	if(proxyReader->fProxyReaderTable == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(proxyReader->fProxyReaderTable->transmit == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}
	if(inRequests == NULL) {return INVALID_PARAMETER_ERROR;}
	if(outResponses == NULL) {return INVALID_PARAMETER_ERROR;}
	if(inRequests->fKeepChannelOpenFlag == 0 && proxyReader->fProxyReaderTable->closeChannel == NULL) {return FUNCTION_TABLE_NOT_INITIALIZED_ERROR;}


	if(inRequests->fAidToSelectLength >= 5) {

		status = selectApplication(
			proxyReader, 
			inRequests->fAidToSelect, 
			inRequests->fAidToSelectLength, 
			outResponses->fFci, 
			&(outResponses->fFciLength));

		if(status != EXECUTION_OK) {
			return status;
		}

	} else {

		OsKpl_Memcpy(outResponses->fAtr, proxyReader->fSeAtr, proxyReader->fSeAtrLength);
		outResponses->fAtrLength = proxyReader->fSeAtrLength;
	}

	for(i = 0; i < inRequests->fApduRequests.fNumberOfApdus; i++) {

		status = proxyReader->fProxyReaderTable->transmit(
			inProxyReader,
			inRequests->fApduRequests.fApduList[i].fApdu,
			inRequests->fApduRequests.fApduList[i].fApduLength,
			outResponses->fApduResponses.fApduList[i].fApdu,
			&(outResponses->fApduResponses.fApduList[i].fApduLength));


		if(status != EXECUTION_OK) {

			outResponses->fApduResponses.fApduList[i].fApduStatus = K_CMD_STATUS_FAILED;

			if(proxyReader->fTransmitBreakOnFailFlag) {
				return IO_READER_ERROR;
			}

		} else {

			if(inRequests->fApduRequests.fApduList[i].fCase4Flag == 1) {

				status = checkResponse(
					proxyReader, 
					outResponses->fApduResponses.fApduList[i].fApdu, 
					&(outResponses->fApduResponses.fApduList[i].fApduLength));

				if(status != EXECUTION_OK) {
					outResponses->fApduResponses.fApduList[i].fApduStatus = K_CMD_STATUS_FAILED;

					if(proxyReader->fTransmitBreakOnFailFlag) {
						return IO_READER_ERROR;
					}
				}
			}

			outResponses->fApduResponses.fApduList[i].fApduStatus = K_CMD_STATUS_SUCCESS;
		}
		
	}

	if(inRequests->fKeepChannelOpenFlag == 0) {
		proxyReader->fProxyReaderTable->closeChannel(proxyReader);
	}

	return EXECUTION_OK;
}
