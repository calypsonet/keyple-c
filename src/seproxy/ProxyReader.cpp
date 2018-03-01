#include "ProxyReader.hpp"
#include "seProxyMngr.h"
#include "osKplMem.h"
#include "osKplStr.h"

namespace keyple {

	namespace seproxy {

		ProxyReader::ProxyReader(std::string inReaderName, void * inPluginContext) {
			readerName = inReaderName;
			pluginContext = inPluginContext;
		}


		ProxyReader::~ProxyReader() {}


		std::string ProxyReader::getName() {return readerName;}


		ExecutionStatus ProxyReader::transmit(SeRequest* inSeApplicationRequest, SeResponse* outSeApplicationResponse) 
		{
			T_SeRequest seRequest;
			T_SeResponse seResponse;
			uint16_t i = 0;
			ExecutionStatus status;

			if(!readerInitialized) {return CHANNEL_STATE_READER_ERROR;}

			OsKpl_Memset(&seRequest, 0, sizeof(T_SeRequest));
			OsKpl_Memset(&seResponse, 0, sizeof(T_SeResponse));

			seRequest.fAidToSelectLength = inSeApplicationRequest->getAidToSelectLength();
			OsKpl_Memcpy(seRequest.fAidToSelect, inSeApplicationRequest->getAidToSelect(), seRequest.fAidToSelectLength); 
			seRequest.fKeepChannelOpenFlag = inSeApplicationRequest->askKeepChannelOpen();

			seRequest.fApduRequests.fNumberOfApdus = inSeApplicationRequest->getApduRequests()->size();
			seRequest.fApduRequests.fApduList = (T_ApduCommandElement*) OsKpl_Malloc(seRequest.fApduRequests.fNumberOfApdus * sizeof(T_ApduCommandElement));

			if(seRequest.fApduRequests.fApduList == NULL) {return OUT_OF_MEMORY_ERROR;}

			seResponse.fApduResponses.fNumberOfApdus = inSeApplicationRequest->getApduRequests()->size();
			seResponse.fApduResponses.fApduList = (T_ApduResponseElement*) OsKpl_Malloc(seRequest.fApduRequests.fNumberOfApdus * sizeof(T_ApduResponseElement));

			if(seResponse.fApduResponses.fApduList == NULL) {
				OsKpl_Free(seRequest.fApduRequests.fApduList);
				return OUT_OF_MEMORY_ERROR;
			}

			for(std::list<ApduRequest>::iterator apduRequestIter = inSeApplicationRequest->getApduRequests()->begin();
				apduRequestIter != inSeApplicationRequest->getApduRequests()->end();
				apduRequestIter++, i++)
			{
				OsKpl_Memcpy(seRequest.fApduRequests.fApduList[i].fApdu, apduRequestIter->getApdu(), apduRequestIter->getApduLength());
				seRequest.fApduRequests.fApduList[i].fApduLength = apduRequestIter->getApduLength();
				seRequest.fApduRequests.fApduList[i].fCase4Flag = apduRequestIter->isCase4();
			}

			status = transmitRequests(proxyReaderContext, &seRequest, &seResponse);
			
			ApduResponse fci(seResponse.fFci, seResponse.fFciLength, true);
			outSeApplicationResponse->setData(
				seResponse.fChannelPreviouslyOpenFlag, 
				&fci, 
				seResponse.fAtr,
				seResponse.fAtrLength);
			
			for(i = 0; i < seResponse.fApduResponses.fNumberOfApdus; i++) {

				ApduResponse response(seResponse.fApduResponses.fApduList[i].fApdu,
					seResponse.fApduResponses.fApduList[i].fApduLength,
					seResponse.fApduResponses.fApduList[i].fApduStatus == K_CMD_STATUS_SUCCESS);

				outSeApplicationResponse->getApduResponses()->push_back(response);
			}

			OsKpl_Free(seRequest.fApduRequests.fApduList);
			OsKpl_Free(seResponse.fApduResponses.fApduList);

			return EXECUTION_OK;
		}


		bool ProxyReader::isSePresent() {

			ExecutionStatus status;

			if(!readerInitialized) {

				status = getReaderByDevName(pluginContext, (int8_t*) readerName.c_str(), &proxyReaderContext);

				if(status != EXECUTION_OK) {return false;}

				readerInitialized = true;
			}

			status = getSeStatus(proxyReaderContext);

			if(status != EXECUTION_OK) {return false;}

			return true;
		}

		std::map<std::string, std::string>* ProxyReader::getParameters() {

			T_ProxyReaderParameter *readerParameters = NULL;
			uint32_t numberOfParameters;
			ExecutionStatus status;
			
			if(!readerInitialized) {

				status = getReaderByDevName(pluginContext, (int8_t*) readerName.c_str(), &proxyReaderContext);

				if(status != EXECUTION_OK) {return NULL;}

				readerInitialized = true;
			}

			status = getReaderParameters(proxyReaderContext, NULL, &numberOfParameters);
			if(status != EXECUTION_OK) {return NULL;}

			readerParameters = (T_ProxyReaderParameter*) OsKpl_Malloc(sizeof(T_ProxyReaderParameter) * numberOfParameters);
			if(readerParameters == NULL) {return NULL;}

			status = getReaderParameters(proxyReaderContext, readerParameters, &numberOfParameters);
			if(status != EXECUTION_OK) {
				OsKpl_Free(readerParameters);
				return NULL;}

			for(uint32_t i = 0; i < numberOfParameters; i++) {
				configurationMap[std::string((const char*) readerParameters[i].fParameterName)] = std::string((const char*) readerParameters[i].fParameterValue);
			}

			OsKpl_Free(readerParameters);
			return &configurationMap;
		}


		ExecutionStatus ProxyReader::setAParameter(std::string inKey, std::string inValue) {

			T_ProxyReaderParameter readerParameterToSet;
			ExecutionStatus status = EXECUTION_OK;

			if(!readerInitialized) {

				status = getReaderByDevName(pluginContext, (int8_t*) readerName.c_str(), &proxyReaderContext);

				if(status != EXECUTION_OK) {return status;}

				readerInitialized = true;
			}

			OsKpl_Strcpy(readerParameterToSet.fParameterName, (int8_t*) (inKey.c_str()));
			OsKpl_Strcpy(readerParameterToSet.fParameterValue, (int8_t*) (inValue.c_str()));

			status = setReaderParameters(proxyReaderContext, &readerParameterToSet, 1);

			return status;
		}


		ExecutionStatus ProxyReader::setParameters(std::map<std::string, std::string>* inSettings) {

			T_ProxyReaderParameter *readerParametersToSet = NULL;
			ExecutionStatus status = EXECUTION_OK;

			if(inSettings->size() <= 0) {return PARAMETER_PROCESSING_ERROR_ERROR;}

			if(!readerInitialized) {

				status = getReaderByDevName(pluginContext, (int8_t*) readerName.c_str(), &proxyReaderContext);

				if(status != EXECUTION_OK) {return status;}

				readerInitialized = true;
			}

			std::map<std::string, std::string>::iterator configIter = inSettings->begin();

			readerParametersToSet = (T_ProxyReaderParameter*) OsKpl_Malloc(sizeof(T_ProxyReaderParameter) * inSettings->size());

			if(readerParametersToSet == NULL) {return OUT_OF_MEMORY_ERROR;}

			uint32_t i = 0;
			while(configIter != inSettings->end()) {
				OsKpl_Strcpy(readerParametersToSet[i].fParameterName, (int8_t*) (configIter->first.c_str()));
				OsKpl_Strcpy(readerParametersToSet[i].fParameterValue, (int8_t*) (configIter->second.c_str()));
				configIter++;
			}

			status = setReaderParameters(proxyReaderContext, readerParametersToSet, inSettings->size());

			OsKpl_Free(readerParametersToSet);

			return status;
		}

	}
}
