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
 * \file keyple_reader_interface.c.
 *
 * \brief Keyple reader interface
 */
#include <string.h>

#include "keyple_config.h"
#include "keyple_log.h"
#include "keyple_reader_interface.h"
#include "keyple_status.h"

static struct
{
    uint8_t *addressStart;
    uint8_t *nextFree;
    uint16_t size;
} m_MemoryPools[MAX_NBR_POOLS];

static struct
{
    struct readerDescriptor_s reader;
    struct cardInfo_s currentCardInfo;
} m_Readers[MAX_NBR_READERS];

ExecutionStatus readerInterfaceInit()
{
    /// blank statics structures
    memset(m_MemoryPools, 0, sizeof(m_MemoryPools));
    memset(m_Readers, 0, sizeof(m_Readers));

    return EXECUTION_OK;
}

ExecutionStatus provideMemoryPool(uint16_t inPoolIndex, uint8_t *inPool, uint16_t inSize)
{
    ExecutionStatus status = INDEX_OUTSIDE_BOUNDARIES;
    if (inPoolIndex < MAX_POOL_INDEX)
    {
        m_MemoryPools[inPoolIndex].addressStart = inPool;
        m_MemoryPools[inPoolIndex].nextFree     = inPool;
        m_MemoryPools[inPoolIndex].size         = inSize;
        status                                  = EXECUTION_OK;
    }
    return status;
}

ExecutionStatus linkNewApdu(uint16_t inPoolIndex, struct apduCommand_s **outNewCmd,
                            struct apduCommand_s **inPrevCmd, uint16_t inCmdDataLength,
                            struct apduResponse_s **outNewResp, struct apduResponse_s **inPrevResp,
                            uint16_t inRespDataLength)
{
    ExecutionStatus status = INDEX_OUTSIDE_BOUNDARIES;
    *outNewCmd             = NULL;
    *outNewResp            = NULL;
    if (inPoolIndex <= MAX_POOL_INDEX)
    {
        /// start from the begining of the pool if not linked
        if (inPrevCmd == NULL || inPrevResp == NULL)
        {
            m_MemoryPools[inPoolIndex].nextFree = m_MemoryPools[inPoolIndex].addressStart;
        }
        /// compute remaining size = pool size - (last - start)
        uint16_t remainingSize = m_MemoryPools[inPoolIndex].size - (m_MemoryPools[inPoolIndex].nextFree -
                                                                    m_MemoryPools[inPoolIndex].addressStart);
        /// check if there is enough space
        if (sizeof(struct apduCommand_s) + inCmdDataLength + sizeof(struct apduResponse_s) +
                inRespDataLength <=
            remainingSize)
        {
            // allocate command
            *outNewCmd = (struct apduCommand_s *)((uint32_t)m_MemoryPools[inPoolIndex].nextFree);

            // link to previous if needed
            if (inPrevCmd != NULL)
            {
                (*inPrevCmd)->next = *outNewCmd;
            }

            /// initialize new command structure fields
            (*outNewCmd)->isCase4 = false;
            (*outNewCmd)->len     = 0;
            (*outNewCmd)->data    = (uint8_t *)(*outNewCmd) + sizeof(struct apduCommand_s);
            (*outNewCmd)->next    = NULL;

            // allocate response
            *outNewResp = (struct apduResponse_s *)((uint8_t *)(*outNewCmd) + sizeof(struct apduResponse_s) +
                                                    inCmdDataLength);
            // link to previous if needed
            if (inPrevResp != NULL)
            {
                (*inPrevResp)->next = *outNewResp;
            }

            /// initialize new response structure fields
            (*outNewResp)->len  = inRespDataLength;
            (*outNewResp)->data = (uint8_t *)(*outNewResp) + sizeof(struct apduCommand_s);
            (*outNewResp)->next = NULL;

            /// update pool
            m_MemoryPools[inPoolIndex].nextFree = m_MemoryPools[inPoolIndex].nextFree +
                                                  sizeof(struct apduCommand_s) + inCmdDataLength +
                                                  sizeof(struct apduResponse_s) + inRespDataLength;

            status = EXECUTION_OK;
        }
        else
        {
            status = OUT_OF_MEMORY_ERROR;
        }
    }

    return status;
}

ExecutionStatus setReader(uint16_t inIndex, struct readerDescriptor_s inReader)
{
    ExecutionStatus status = INDEX_OUTSIDE_BOUNDARIES;
    if (inIndex <= MAX_READER_INDEX)
    {
        memcpy(&m_Readers[inIndex].reader, &inReader, sizeof(struct readerDescriptor_s));
        status = EXECUTION_OK;
    }
    return status;
}

ExecutionStatus executeRequest(uint16_t inIndex, struct requestParameters_s inRequestParameters,
                               struct apduCommand_s *inCommandApdu, struct apduResponse_s *outResponseApdu)
{
    ExecutionStatus status = INDEX_OUTSIDE_BOUNDARIES;

    struct apduCommand_s *currentCommandApdu   = inCommandApdu;
    struct apduResponse_s *currentResponseApdu = outResponseApdu;
    struct cardInfo_s cardInfo;

    m_Readers[inIndex].reader.openChannel(&cardInfo);

    if (inIndex <= MAX_READER_INDEX)
    {
        status = EXECUTION_OK;

        while (currentCommandApdu != NULL && currentResponseApdu != NULL)
        {
            uint16_t transmitStatus;
            transmitStatus =
                m_Readers[inIndex].reader.transmit(currentCommandApdu->data, currentCommandApdu->len,
                                                   currentResponseApdu->data, &currentResponseApdu->len);
            if (transmitStatus != 0)
            {
                status = IO_READER_ERROR;
                break;
            }
            currentCommandApdu  = currentCommandApdu->next;
            currentResponseApdu = currentResponseApdu->next;
        }
    }
    return status;
}
