/**
Summary
*/
#ifndef SE_PROXY_H
#define SE_PROXY_H

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */
#include "osKplCfg.h"
#include "ExecutionStatus.hpp"


/* -------------------------------------------------------------------------- * 
 * - Defines ---------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#define K_PROXY_READER_MAX_PARAMETER_TEXT_LENGTH							50

#define K_APDU_MAX_LENGTH													261

#define K_AID_MAX_SIZE														16

#define K_ATR_MAX_LENGTH													36

#define K_FCI_MAX_LENGTH													46

/* -------------------------------------------------------------------------- * 
 * - Type Definitions ------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

typedef enum {

	K_PLUGIN_TYPE_PCSC = 0,

	// Unknown Plugin
	K_PLUGIN_TYPE_UKNOWN = 0xFF

} T_PluginType;


typedef enum {

	K_DEBUG_MODE_ON,

	K_DEBUG_MODE_OFF

} T_DebugMode;


typedef struct {

	int8_t fParameterName[K_PROXY_READER_MAX_PARAMETER_TEXT_LENGTH];

	int8_t fParameterValue[K_PROXY_READER_MAX_PARAMETER_TEXT_LENGTH];

} T_ProxyReaderParameter;


typedef enum
{
	// Command has not been executed
	K_CMD_STATUS_NOT_EXECUTED = 0,

	// Command was executed with success
	K_CMD_STATUS_SUCCESS,

	// An error was returned by the reader during command execution
	K_CMD_STATUS_FAILED,

	// A timeout occurred when executing the command
	K_CMD_STATUS_NO_RESPONSE

} T_CommandStatus;


typedef struct {

	uint8_t fApdu[K_APDU_MAX_LENGTH];

	uint16_t fApduLength;

	uint8_t fCase4Flag;

} T_ApduCommandElement;


typedef struct {

	uint8_t fApdu[K_APDU_MAX_LENGTH];

	uint16_t fApduLength;

	T_CommandStatus fApduStatus;

} T_ApduResponseElement;


typedef struct {

	uint32_t fNumberOfApdus;

	T_ApduCommandElement *fApduList;

} T_ApduCommandList;


typedef struct {

	uint32_t fNumberOfApdus;

	T_ApduResponseElement *fApduList;

} T_ApduResponseList;


typedef struct {

	uint8_t fAidToSelect[K_AID_MAX_SIZE];

	uint16_t fAidToSelectLength;

	uint8_t fKeepChannelOpenFlag;

	T_ApduCommandList fApduRequests;

} T_SeRequest;  


typedef struct {

	uint8_t fChannelPreviouslyOpenFlag;

	uint8_t fFci[K_FCI_MAX_LENGTH];

	uint16_t fFciLength;

	uint8_t fAtr[K_ATR_MAX_LENGTH];

	uint16_t fAtrLength;

	T_ApduResponseList fApduResponses;

} T_SeResponse;

#endif //SE_PROXY_H