/**
Summary
*/

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#include <stdio.h>
#include "seProxyMngr.h"
#include "osKplMem.h"
#include "osKplStr.h"

#include "SeProxyService.hpp"


/* -------------------------------------------------------------------------- * 
 * - Global Variables ------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
T_ApduCommandElement commandList[] = {
	{{0x94, 0x20, 0x80, 0x00, 0x04, 0x30, 0x30, 0x30, 0x30}, 9, 0},
	{{0x94, 0x8A, 0x89, 0x38, 0x04, 0x17, 0x00, 0x58, 0xA5}, 9, 0},
	{{0x94, 0xB2, 0x01, 0x1C, 0x18}, 5, 0},
	{{0x94, 0xB2, 0x01, 0x4C, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x02, 0x4C, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x03, 0x4C, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x04, 0x4C, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x01, 0x44, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x02, 0x44, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x03, 0x44, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x01, 0xEC, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x01, 0xF4, 0x1D}, 5, 0},
	{{0x94, 0xB2, 0x01, 0xCC, 0x1D}, 5, 0} };

uint16_t commandListLength = 13;

T_ApduCommandElement csmCommandList[] = {
	{{0x94, 0xBE, 0x00, 0xA0, 0x30}, 5, 0},
	{{0xF0, 0x2A, 0x9E, 0x9A, 0x15, 0x60, 0x02, 0x08, 0x08, 0x09, 0xB1, 0x98, 0xD8, 0x00, 0xFF, 0x2A, 0x42, 0x02, 0x7C, 0xD6, 0xE9, 0x5D, 0x40, 0x00, 0x00, 0x46}, 26, 1}, //84DD9000
	{{0xF0, 0x2A, 0x00, 0xA8, 0x17, 0x60, 0x02, 0x08, 0x08, 0x09, 0xB1, 0x98, 0xD8, 0x00, 0xFF, 0x2B, 0x42, 0x02, 0x7C, 0xD6, 0xE9, 0x5D, 0x40, 0x00, 0x00, 0x46, 0x84, 0xDD}, 28, 0}, //9000
	{{0xF0, 0x2A, 0x86, 0x80, 0x15, 0x60, 0x02, 0x08, 0x08, 0x09, 0xB1, 0x98, 0xD8, 0x00, 0xFF, 0x2E, 0x41, 0x00, 0x8C, 0x00, 0x16, 0xC7, 0xEC, 0x00, 0xC0, 0x31}, 26, 1}, //7CD6E9A3C95D4CA59000
	{{0xF0, 0x2A, 0x80, 0x86, 0x15, 0x60, 0x02, 0x08, 0x08, 0x09, 0xB1, 0x98, 0xD8, 0x00, 0xFF, 0x2E, 0x41, 0x00, 0x7C, 0xD6, 0xE9, 0xA3, 0xC9, 0x5D, 0x4C, 0xA5}, 26, 1} }; //8C0016C7EC00C0319000

uint16_t csmCommandListLength = 5;

T_ApduResponseElement csmResponseList[] = {
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED}};


T_ApduResponseElement responseList[] = {
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED},
	{{0}, K_APDU_MAX_LENGTH, K_CMD_STATUS_NOT_EXECUTED} };

T_ProxyReaderParameter readerParametersToSet[] = {{"share_mode", "shared"},{"transmit_break_on_fail", "true"}, {"proxy_reader_type", "po_reader"}};

uint8_t aidToSelect[] = {0x31, 0x54, 0x49, 0x43, 0x2E, 0x49, 0x43, 0x41};
uint16_t aidToSelectLength = 8;

/* -------------------------------------------------------------------------- * 
 * - External Functions ----------------------------------------------------- *
 * -------------------------------------------------------------------------- */

static void printByteArray(int8_t *inName, int16_t inIndex, uint8_t *inByteArray, uint16_t inByteArrayLength) 
{
	uint16_t i;

	if(inIndex >= 0) {
		printf("\t\t === %s[%d]:: ", inName, inIndex);
	} else {
		printf("\t\t === %s:: ", inName);
	}

	for(i = 0; i < inByteArrayLength; i++) {
		printf("%02X", inByteArray[i]);
	}
	printf(" ===\n");
}


void test_c_interface()
{
	void *proxyContext;
	void *pluginContext;
	void *proxyReader;
	void *csmProxyReader;
	uint16_t proxyMngrVersion = 0;
	uint16_t numberOfPlugins = 0;
	uint16_t i;
	T_PluginType *pluginList = NULL;
	ExecutionStatus status;
	int8_t *readersMultiString = NULL;
	uint32_t readerMultiStringSize = 0;
	uint32_t iterReaders;
	uint32_t readersCount = 1;
	int8_t pluginName[100];
	T_ProxyReaderParameter *readerParameters;
	uint32_t numberOfParameters;

	// Configurable parameters
	int8_t csmReaderName[] = "Identive CLOUD 4710 F SAM Reader 0";

	T_SeRequest seRequest;
	T_SeResponse seResponse;

	status = initProxyManager(K_DEBUG_MODE_ON, (int8_t*)"", &proxyMngrVersion, &proxyContext);
	printf("initProxyManager::status::%02X\n", status);

	printf("\t\t === Version::%d ===\n", proxyMngrVersion);

	status = getSupportedPlugins(proxyContext, pluginList, &numberOfPlugins);
	printf("getSupportedPlugins(1)::status::%02X\n", status);

	pluginList = (T_PluginType*) OsKpl_Malloc(sizeof(T_PluginType) * numberOfPlugins);

	status = getSupportedPlugins(proxyContext, pluginList, &numberOfPlugins);
	printf("getSupportedPlugins(2)::status::%02X\n", status);

	printf("\t\t === NumberOfSupportedPlugins::%d ===\n", numberOfPlugins);
	for(i = 0; i < numberOfPlugins; i++) 
	{
		printf("\t\t === Plugin[%d]::Type::%02X ===\n", i, pluginList[i]);
	}

	status = getPlugin(proxyContext, pluginList[0], &pluginContext);
	printf("getPlugin::status::%02X\n", status);

	status = getPluginName(pluginContext, pluginName);
	printf("getPluginName::status::%02X\n", status);
	printf("\t\t === Plugin Name::%s ===\n", pluginName);

	status = getReaderDevNames(pluginContext, readersMultiString, &readerMultiStringSize);
	printf("getReaderDevNames(1)::status::%02X\n", status);

	readersMultiString = (int8_t*) OsKpl_Malloc(readerMultiStringSize);

	status = getReaderDevNames(pluginContext, readersMultiString, &readerMultiStringSize);
	printf("getReaderDevNames(2)::status::%02X\n", status);

	for(iterReaders = 0; iterReaders < readerMultiStringSize; iterReaders++) {
		if(OsKpl_Strlen(readersMultiString+iterReaders) > 1) {
			printf("\t\t === Reader[%d]::%s ===\n", readersCount, readersMultiString+iterReaders);
		}
		iterReaders += OsKpl_Strlen(readersMultiString+iterReaders);
		readersCount++;
	}

	status = getReaderByDevName(pluginContext, readersMultiString, &proxyReader);
	printf("getReaderByDevName::status::%02X\n", status);

	status = getReaderParameters(proxyReader, NULL, &numberOfParameters);
	printf("getParameters(1)::status::%02X\n", status);

	readerParameters = (T_ProxyReaderParameter*) OsKpl_Malloc(sizeof(T_ProxyReaderParameter) * numberOfParameters);

	status = getReaderParameters(proxyReader, readerParameters, &numberOfParameters);
	printf("getParameters(2)::status::%02X\n", status);

	printf("\t\t === Reader Parameters::#%d ===\n", numberOfParameters);

	for(i = 0; i < numberOfParameters; i++) {
		printf("\t\t === Parameter[%d]::{%s}-{%s}\n", i, readerParameters[i].fParameterName, readerParameters[i].fParameterValue);
	}

	status = setReaderParameters(proxyReader, readerParametersToSet, 3);
	printf("setParameters::status::%02X\n", status);

	status = getReaderParameters(proxyReader, readerParameters, &numberOfParameters);
	printf("getParameters(3)::status::%02X\n", status);

	printf("\t\t === Reader Parameters::#%d ===\n", numberOfParameters);

	for(i = 0; i < numberOfParameters; i++) {
		printf("\t\t === Parameter[%d]::{%s}-{%s}\n", i, readerParameters[i].fParameterName, readerParameters[i].fParameterValue);
	}

	OsKpl_Free(readerParameters);

	status = getSeStatus(proxyReader);
	printf("getSeData::status::%02X\n", status);

	OsKpl_Memset(&seRequest, 0, sizeof(T_SeRequest));
	OsKpl_Memset(&seResponse, 0, sizeof(T_SeResponse));

	seRequest.fApduRequests.fNumberOfApdus = commandListLength;
	seRequest.fApduRequests.fApduList = commandList;
	seRequest.fKeepChannelOpenFlag = 0;
	OsKpl_Memcpy(seRequest.fAidToSelect, aidToSelect, aidToSelectLength);
	seRequest.fAidToSelectLength = aidToSelectLength;

	seResponse.fApduResponses.fNumberOfApdus = commandListLength;
	seResponse.fApduResponses.fApduList = responseList;

	status = transmitRequests(proxyReader, &seRequest, &seResponse);
	printf("transmitRequests::status::%02X\n", status);

	printf("\t\t === Commands Exchanged with PO:: ===\n");
	printByteArray((int8_t*)"FCI", -1, seResponse.fFci, seResponse.fFciLength);
	for(i = 0; i < seRequest.fApduRequests.fNumberOfApdus; i++)
	{
		printByteArray((int8_t*)"Command", i, seRequest.fApduRequests.fApduList[i].fApdu, seRequest.fApduRequests.fApduList[i].fApduLength);

		if(seResponse.fApduResponses.fApduList[i].fApduStatus == K_CMD_STATUS_FAILED) {
			printf("\t\t === Command failed! ===\n");
		} else if(seResponse.fApduResponses.fApduList[i].fApduStatus == K_CMD_STATUS_NOT_EXECUTED) {
			printf("\t\t === Command not sent! ===\n");
		} else {
			printByteArray((int8_t*)"Response", i, seResponse.fApduResponses.fApduList[i].fApdu, seResponse.fApduResponses.fApduList[i].fApduLength);
		}
	}

	status = getReaderByDevName(pluginContext, csmReaderName, &csmProxyReader);
	printf("transmit::getReaderByDevName(CSM)::%02X\n", status);

	status = getSeStatus(csmProxyReader);
	printf("getSeData(CSM)::status::%02X\n", status);

	OsKpl_Memset(&seRequest, 0, sizeof(T_SeRequest));
	OsKpl_Memset(&seResponse, 0, sizeof(T_SeResponse));

	seRequest.fApduRequests.fNumberOfApdus = csmCommandListLength;
	seRequest.fApduRequests.fApduList = csmCommandList;
	seRequest.fKeepChannelOpenFlag = 0;

	seResponse.fApduResponses.fNumberOfApdus = csmCommandListLength;
	seResponse.fApduResponses.fApduList = csmResponseList;

	status = transmitRequests(csmProxyReader, &seRequest, &seResponse);
	printf("transmitRequests(CSM)::status::%02X\n", status);

	printf("\t\t === Commands Exchanged with CSM:: ===\n");
	printByteArray((int8_t*)"ATR", -1, seResponse.fAtr, seResponse.fAtrLength);
	for(i = 0; i < seRequest.fApduRequests.fNumberOfApdus; i++)
	{
		printByteArray((int8_t*)"Command", i, seRequest.fApduRequests.fApduList[i].fApdu, seRequest.fApduRequests.fApduList[i].fApduLength);

		if(seResponse.fApduResponses.fApduList[i].fApduStatus == K_CMD_STATUS_FAILED) {
			printf("\t\t === Command failed! ===\n");
		} else if(seResponse.fApduResponses.fApduList[i].fApduStatus == K_CMD_STATUS_NOT_EXECUTED) {
			printf("\t\t === Command not sent! ===\n");
		} else {
			printByteArray((int8_t*)"Response", i, seResponse.fApduResponses.fApduList[i].fApdu, seResponse.fApduResponses.fApduList[i].fApduLength);
		}
	}

	OsKpl_Memset(&seRequest, 0, sizeof(T_SeRequest));
	OsKpl_Memset(&seResponse, 0, sizeof(T_SeResponse));

	OsKpl_Free(readersMultiString);

	OsKpl_Free(pluginList);

	status = terminateProxyManager(proxyContext);
	printf("terminateProxyManager::status::%02X\n", status);

	printf("===========================================\n");
	printf("= Mallocs to Free::%d\n", OsKpl_MemoryState());
	printf("===========================================\n");
}


void test_cpp_interface()
{
	ExecutionStatus status;

	keyple::seproxy::SeProxyService *proxyService;
	proxyService = proxyService->getInstance();

	printf("\t\t === Version::%d ===\n", proxyService->getVersion());

	std::list<keyple::seproxy::ReadersPlugin>* plugins = proxyService->getPlugins();

	printf("\t\t === NumberOfSupportedPlugins::%d ===\n", plugins->size());

	std::list<keyple::seproxy::ReadersPlugin>::iterator pluginIter = plugins->begin();

	printf("\t\t === Plugin Name::%s ===\n", pluginIter->getName().c_str());

	std::list<keyple::seproxy::ProxyReader>* proxyReaders = pluginIter->getReaders();

	printf("\t\t === NumberOfProxyReaders::%d ===\n", proxyReaders->size());

	if(proxyReaders->size() < 2) {
		printf("At least 2 PC/SC readers are required for executing this test program!\n");
		return;
	}

	std::list<keyple::seproxy::ProxyReader>::iterator readerIterator = proxyReaders->begin();

	for(uint16_t i = 0; i < proxyReaders->size(); i++) {
		printf("\t\t === Reader[%d]::%s ===\n", i+1, readerIterator->getName().c_str());
		readerIterator++;
	}

	readerIterator = proxyReaders->begin();

	printf("\n\t\t =============================================================\n");
	printf("\t\t = Reader 1\n");
	printf("\t\t =============================================================\n\n");

	std::map<std::string, std::string>* poReaderConfiguration = readerIterator->getParameters();

	std::map<std::string, std::string>::iterator configIter = poReaderConfiguration->begin();

	printf("\t\t === Reader Parameters::#%d ===\n", poReaderConfiguration->size());

	uint32_t i = 1;
	while(configIter != poReaderConfiguration->end())
	{
		printf("\t\t === Parameter[%d]::{%s}-{%s}\n", i, configIter->first.c_str(), configIter->second.c_str());
		i++;
		configIter++;
	}

	std::map<std::string, std::string> poReaderConfigurationToSet;

	for(i = 0; i < 3; i++) {
		poReaderConfigurationToSet[std::string((const char*) readerParametersToSet[i].fParameterName)] = std::string((const char*) readerParametersToSet[i].fParameterValue);
	}

	status = readerIterator->setParameters(&poReaderConfigurationToSet);

	poReaderConfiguration = readerIterator->getParameters();

	configIter = poReaderConfiguration->begin();

	printf("\t\t === Reader Parameters::#%d ===\n", poReaderConfiguration->size());

	i = 1;
	while(configIter != poReaderConfiguration->end())
	{
		printf("\t\t === Parameter[%d]::{%s}-{%s}\n", i, configIter->first.c_str(), configIter->second.c_str());
		i++;
		configIter++;
	}

	printf("\t\t === isSePresent::%d ===\n", readerIterator->isSePresent());

	std::list<keyple::seproxy::ApduRequest> apduRequestList;
	for(uint16_t i = 0; i < commandListLength; i++) {
		keyple::seproxy::ApduRequest request(commandList[i].fApdu, commandList[i].fApduLength, (bool) commandList[i].fCase4Flag);
		apduRequestList.push_back(request);
	}

	keyple::seproxy::SeRequest poRequest(aidToSelect, aidToSelectLength, &apduRequestList, false);

	keyple::seproxy::SeResponse poResponse;

	status = readerIterator->transmit(&poRequest, &poResponse);
	printf("transmit::status::%02X\n", status);

	printf("\t\t === Commands Exchanged with PO:: ===\n");
	printByteArray((int8_t*)"FCI", -1, poResponse.getFci()->getApdu(), poResponse.getFci()->getApduLength());

	std::list<keyple::seproxy::ApduRequest>::iterator poCommandIter = poRequest.getApduRequests()->begin();
	std::list<keyple::seproxy::ApduResponse>::iterator poResponseIter = poResponse.getApduResponses()->begin();
	for(uint16_t i = 0; i < poResponse.getApduResponses()->size(); i++, poCommandIter++, poResponseIter++)
	{
		printByteArray((int8_t*)"Command", i, poCommandIter->getApdu(), poCommandIter->getApduLength());

		if(!poResponseIter->isSuccessful()) {
			printf("\t\t === Command failed! ===\n");
		} else {
			printByteArray((int8_t*)"Response", i, poResponseIter->getApdu(), poResponseIter->getApduLength());
		}
	}

	readerIterator++;

	printf("\n\t\t =============================================================\n");
	printf("\t\t = Reader 2\n");
	printf("\t\t =============================================================\n\n");

	printf("\t\t === isSePresent::%d ===\n", readerIterator->isSePresent());

	std::list<keyple::seproxy::ApduRequest> csmApduRequestList;
	for(uint16_t i = 0; i < csmCommandListLength; i++) {
		keyple::seproxy::ApduRequest request(csmCommandList[i].fApdu, csmCommandList[i].fApduLength, (bool) csmCommandList[i].fCase4Flag);
		csmApduRequestList.push_back(request);
	}

	keyple::seproxy::SeRequest csmRequest(NULL, 0, &csmApduRequestList, false);

	keyple::seproxy::SeResponse csmResponse;

	status = readerIterator->transmit(&csmRequest, &csmResponse);
	printf("transmit::status::%02X\n", status);

	printf("\t\t === Commands Exchanged with CSM:: ===\n");
	printByteArray((int8_t*)"ATR", -1, csmResponse.getAtr(), csmResponse.getAtrLength());

	std::list<keyple::seproxy::ApduRequest>::iterator csmCommandIter = csmRequest.getApduRequests()->begin();
	std::list<keyple::seproxy::ApduResponse>::iterator csmResponseIter = csmResponse.getApduResponses()->begin();
	for(uint16_t i = 0; i < csmResponse.getApduResponses()->size(); i++, csmCommandIter++, csmResponseIter++)
	{
		printByteArray((int8_t*)"Command", i, csmCommandIter->getApdu(), csmCommandIter->getApduLength());

		if(!csmResponseIter->isSuccessful()) {
			printf("\t\t === Command failed! ===\n");
		} else {
			printByteArray((int8_t*)"Response", i, csmResponseIter->getApdu(), csmResponseIter->getApduLength());
		}
	}

}


int main(int argc, char *argv[])
{
	test_cpp_interface();

	//test_c_interface();

	return 0;
}
