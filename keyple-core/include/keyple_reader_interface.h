/*
 * Copyright (c) 2018 Calypso Networks Association
 * https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License version 2.0 which
 * accompanies this distribution, and is available at
 * https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
///< .
 * \file keyple_reader_interface.h.
 *
 * \brief Declares the structures used with the keyple reader interface
 */

#ifndef KEYPLE_READER_INTERFACE_H
#define KEYPLE_READER_INTERFACE_H

#include "keyple_status.h"

/// Reader status codes (should be an enum?)
#define KPL_RDR_SUCCESS 0
#define KPL_RDR_READER_NOT_FOUND 1
#define KPL_RDR_READER_ERROR 2
#define KPL_RDR_PARAM_OUT_OF_BOUNDARIES 3
#define KPL_RDR_CARD_CONNECTION_ERROR 4
#define KPL_RDR_CARD_INFO_ERROR 5
#define KPL_RDR_CARD_DISCONNECTION_ERROR 6
#define KPL_RDR_CARD_TRANSMIT_ERROR 7
#define KPL_RDR_MEMORY_ERROR 8

/// Limits
#define APDU_DATA_LENGTH_MAX 261
#define APDU_RESP_DATA_LENGTH_MAX 258
#define ATR_MAX_LENGTH 32
#define MAX_APDU_PER_REQUEST 8
#define AID_MAX_LENGTH 16 // TODO check this value

/*!
 * \struct cardInfo_s
 *
 * \brief Holds the information retrieved during
 * 		  the establishment of the card connection.
 */

struct cardInfo_s
{
    uint16_t cardStatus;
    uint16_t atrLength;
    uint8_t atr[ATR_MAX_LENGTH];
    uint16_t cardExtraInfoLength;
    uint8_t *cardExtraInfo; /// BER TLV string?
};

/*!
 * \struct readerDescriptor_s
 *
 * \brief Provides the function pointers to handle
 * 		  a reader from the Keyple SDK.
 */

struct readerDescriptor_s
{
    uint16_t (*openChannel)(struct cardInfo_s *outCardInfo);
    uint16_t (*closeChannel)(uint8_t inDisposition);
    uint16_t (*transmit)(uint8_t *inCommand, uint16_t inCommandLength, uint8_t *outResponse,
                         uint16_t *inOutResponseLength);
};

/*!
* \struct  requestParameter_s
*
* \brief   Holds the request parameters such as AID to select, keep channel open flag.
*/

struct requestParameters_s
{
    uint16_t aidToSelectLength;
    uint8_t aidToSelect[AID_MAX_LENGTH];
    bool keepChannelOpen;
};

/*!
 * \struct apduCommand_s
 *
 * \brief Holds the information needed to describe a command APDU.
 */

struct apduCommand_s
{
    uint16_t len;
    uint8_t *data;
    bool isCase4;
    struct apduCommand_s *next;
};

/*!
 * \struct  apduResponse_s
 *
 * \brief   Holds the data received in response to a command APDU.
 */

struct apduResponse_s
{
    uint16_t len;
    uint8_t *data;
    struct apduResponse_s *next;
};

/*!
 * \fn  ExecutionStatus setReader(uint16_t inReaderIndex, struct readerDescriptor_s *inReader);
 *
 * \brief   Provide a reader reader descriptor to be memorised with the associated index \param
 *          [in]  inReaderIndex   The reader index. \param [in] inReader        The reader
 *          descriptor. \return  The execution status.
 *
 * \param           inReaderIndex   Zero-based index of the in reader.
 * \param [in,out]  inReader        If non-null, the in reader.
 *
 * \return  The ExecutionStatus.
 */

ExecutionStatus setReader(uint16_t inReaderIndex, struct readerDescriptor_s inReader);

/*!
 * \fn  ExecutionStatus provideMemoryPool(uint16_t inPoolIndex, uint8_t *inPool, uint16_t inSize);
 *
 * \brief   Provide memory pool to handle memory allocation for commands and responses APDU.
 *
 * \param       inPoolIndex Index of the pool: COMMAND_APDU or RESPONSE_APDU.
 * \param [in]  inPool      The memory pool start address.
 * \param       inSize      Size of the memory pool.
 *
 * \return  The execution status.
 */

ExecutionStatus provideMemoryPool(uint16_t inPoolIndex, uint8_t *inPool, uint16_t inSize);

/*!
 * \fn  ExecutionStatus linkNewApdu(uint16_t inPoolIndex, struct apduCommand_s *outNewCmd, struct apduCommand_s *inPrevCmd, uint16_t inCmdDataLength, struct apduResponse_s *outNewResp, struct apduResponse_s *inPrevResp, uint16_t inRespDataLength);
 *
 * \brief   Allocate new APDU command and response structures and link them to the previously
 *          allocated structures if needed. The specified pool is used for the memory allocation
 *          and can use to differentiate the usage context ; e.g. PO or CSM. If one of the
 *          inPrevxxx pointers is NULL the allocation starts at the beginning of the pool. If
 *          there is not enough space to allocate the requested data, NULL is set as value,
 *          otherwise the addresses of the newly allocated structures are set. The application
 *          should always use the first element of the list.
 *
 * \param           inPoolIndex         Zero-based index of the in pool.
 * \param [in,out]  outNewCmd           If non-null, the in out new command.
 * \param [in]      inPrevCmd           If non-null, the in previous command.
 * \param           inCmdDataLength     Length of the in command data.
 * \param [in,out]  outNewResp          If non-null, the in out new resp.
 * \param [in]      inPrevResp          If non-null, the in previous resp.
 * \param           inRespDataLength    Length of the in resp data.
 *
 * \return  Null if it fails, else a pointer to the apduCommand_s.
 */

ExecutionStatus linkNewApdu(uint16_t inPoolIndex, struct apduCommand_s **outNewCmd,
                            struct apduCommand_s **inPrevCmd, uint16_t inCmdDataLength,
                            struct apduResponse_s **outNewResp, struct apduResponse_s **inPrevResp,
                            uint16_t inRespDataLength);

/*!
 * \fn  ExecutionStatus executeRequest(uint16_t readerIndex, struct requestParameter_s inRequestParameters, struct apduCommand_s *inCommandApdu, struct apduResponse_s *inOutResponseApdu);
 *
 * \brief   Executes the request: send all linked APDU commands and received their responses with
 *          the targeted reader.
 *
 * \param           readerIndex         Index of the reader.
 * \param           inRequestParameters Options for controlling the request (AID to select,
 *                                      channel management). \param [in]      inCommandApdu
 *                                      The first command APDU of a linked list. \param [in,out]
 *                                      inOutResponseApdu   The first response APDU of a linked
 *                                      list.
 * \param [in,out]  inCommandApdu       If non-null, the in command apdu.
 * \param [in,out]  inOutResponseApdu   If non-null, the in out response apdu.
 *
 * \return  The execution status.
 */

ExecutionStatus executeRequest(uint16_t readerIndex, struct requestParameters_s inRequestParameters,
                               struct apduCommand_s *inCommandApdu, struct apduResponse_s *inOutResponseApdu);

#endif // KEYPLE_READER_INTERFACE_H
