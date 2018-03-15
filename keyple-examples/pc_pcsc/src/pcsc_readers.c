/*!
 * \file    pcsc_reader.c
 *
 * \brief   Implements the SDK reader interface with two PC/SC readers
 */
#include "keyple_config.h"
#include "keyple_reader_interface.h"
#include "keyple_log.h"

#include "pcsc_readers.h"

// Module variables
static SCARDCONTEXT m_context = 0;

static struct
{
    char name[READER_NAME_MAX_LENGTH];
    SCARDHANDLE cardHandle;
    unsigned long activeProtocol;
    struct cardInfo_s cardInfo;
} m_readerContext[NBR_READERS];

/// Private reader functions
static uint16_t openChannel(uint32_t readerIndex, struct cardInfo_s *outCardInfo)
{
    uint16_t ret = KPL_RDR_PARAM_OUT_OF_BOUNDARIES; // TODO use constant
    if (readerIndex < NBR_READERS)
    {
        if (SCardConnect(m_context, m_readerContext[readerIndex].name, SCARD_SHARE_SHARED,
                         SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &m_readerContext[readerIndex].cardHandle,
                         &m_readerContext[readerIndex].activeProtocol) == SCARD_S_SUCCESS)
        {
            DWORD cbyte = sizeof(m_readerContext[readerIndex].cardInfo.atr);
            /// Get the ATR buffer as attribute
            if (SCardGetAttrib(m_readerContext[readerIndex].cardHandle, SCARD_ATTR_ATR_STRING,
                               m_readerContext[readerIndex].cardInfo.atr, &cbyte) == SCARD_S_SUCCESS)
            {
                m_readerContext[readerIndex].cardInfo.atrLength = (uint16_t)cbyte;
                LOG_MSG_BUFFER("ATR", m_readerContext[readerIndex].cardInfo.atr,
                               m_readerContext[readerIndex].cardInfo.atrLength);
                *outCardInfo = m_readerContext[readerIndex].cardInfo;
                ret          = KPL_RDR_SUCCESS;
            }
            else
            {
                ret = KPL_RDR_CARD_INFO_ERROR;
            }
        }
        else
        {
            ret = KPL_RDR_CARD_CONNECTION_ERROR;
        }
    }
    return ret;
}

static uint16_t closeChannel(uint32_t readerIndex, uint8_t inDisposition)
{
    uint16_t ret = KPL_RDR_CARD_DISCONNECTION_ERROR;
    if (SCardDisconnect(m_readerContext[readerIndex].cardHandle, inDisposition) == SCARD_S_SUCCESS)
    {
        ret = KPL_RDR_SUCCESS;
    }
    return ret;
}

static uint16_t transmit(uint32_t readerIndex, uint8_t *inCommand, uint16_t inCommandLength,
                         uint8_t *outResponse, uint16_t *inOutResponseLength)
{
    uint16_t ret = KPL_RDR_CARD_TRANSMIT_ERROR;

    LONG pcscStatus;
    DWORD responseLength;
    SCARD_IO_REQUEST scioReq;

    scioReq.dwProtocol  = m_readerContext[readerIndex].activeProtocol;
    scioReq.cbPciLength = sizeof(scioReq);

    responseLength = (unsigned long)*inOutResponseLength;

    LOG_MSG_BUFFER("CMD", inCommand, inCommandLength);

    pcscStatus = SCardTransmit(m_readerContext[readerIndex].cardHandle, &scioReq, inCommand, inCommandLength,
                               NULL, outResponse, &responseLength);

    if (pcscStatus == SCARD_S_SUCCESS)
    {
        LOG_MSG_BUFFER("RESP", outResponse, (uint16_t)responseLength);

        if (responseLength == 2 &&
            (outResponse[0] == 0x61 || outResponse[0] == 0x6C || outResponse[0] == 0x9F))
        {
            /// handle outgoing data: sw1sw2 = 0x61xx, 0x6Cxx or 0x9Fxx
            uint8_t getResponse[] = {0x00, 0xC0, 0x00, 0x00, outResponse[1]};

            responseLength = (unsigned long)*inOutResponseLength;

            LOG_MSG_BUFFER("CMD", getResponse, 5);

            pcscStatus = SCardTransmit(m_readerContext[readerIndex].cardHandle, &scioReq, getResponse, 5,
                                       NULL, outResponse, &responseLength);

            if (pcscStatus == SCARD_S_SUCCESS)
            {
                LOG_MSG_BUFFER("RESP", outResponse, (uint16_t)responseLength);

                *inOutResponseLength = (uint16_t)responseLength;
                ret                  = KPL_RDR_SUCCESS;
            }
        }
        else
        {
            ret = KPL_RDR_SUCCESS;
        }
    }
    return ret;
}

// Public functions
/// PC/SC initialization
uint16_t initPcsc(char *reader0Name, char *reader1Name)
{
    uint16_t ret = KPL_RDR_READER_ERROR;
    LONG pcscStatus;

    pcscStatus = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &m_context);

    if (pcscStatus == SCARD_S_SUCCESS)
    {
        LPTSTR pmszReaders = NULL;
        DWORD cch          = SCARD_AUTOALLOCATE;

        /// Retrieve the readers list.
        ///	m_context was set above by SCardEstablishContext.
        pcscStatus = SCardListReaders(m_context, NULL, (LPTSTR)&pmszReaders, &cch);
        if (pcscStatus == SCARD_S_SUCCESS)
        {
            strcpy(reader0Name, pmszReaders);
            strcpy(reader1Name, pmszReaders + strlen(reader0Name) + 1);

            /// Free the memory.
            if (SCardFreeMemory(m_context, pmszReaders) == SCARD_S_SUCCESS)
            {
                if (reader0Name[0] != '\0' && reader1Name[0] != '\0')
                {
                    ret = KPL_RDR_SUCCESS;
                }
                else
                {
                    ret = KPL_RDR_READER_NOT_FOUND;
                }
            }
            else
            {
                ret = KPL_RDR_MEMORY_ERROR;
            }
        }
    }
    return ret;
}

// reader index - reader name association
uint16_t assignReader(uint32_t readerIndex, char *readerName)
{
    uint16_t ret = KPL_RDR_PARAM_OUT_OF_BOUNDARIES;
    if (readerIndex < NBR_READERS && strlen(readerName) < READER_NAME_MAX_LENGTH)
    {
        strcpy(m_readerContext[readerIndex].name, readerName);
        ret = KPL_RDR_SUCCESS;
    }
    return ret;
}

// Keyple SDK interface functions
uint16_t reader0_openChannel(struct cardInfo_s *outCardInfo)
{
    return openChannel(0, outCardInfo);
}

uint16_t reader0_closeChannel(uint8_t inDisposition)
{
    return closeChannel(0, inDisposition);
}

uint16_t reader0_transmit(uint8_t *inCommand, uint16_t inCommandLength, uint8_t *outResponse,
                          uint16_t *inOutResponseLength)
{
    return transmit(0, inCommand, inCommandLength, outResponse, inOutResponseLength);
}

uint16_t reader1_openChannel(struct cardInfo_s *outCardInfo)
{
    return openChannel(1, outCardInfo);
}

uint16_t reader1_closeChannel(uint8_t inDisposition)
{
    return closeChannel(1, inDisposition);
}

uint16_t reader1_transmit(uint8_t *inCommand, uint16_t inCommandLength, uint8_t *outResponse,
                          uint16_t *inOutResponseLength)
{
    return transmit(1, inCommand, inCommandLength, outResponse, inOutResponseLength);
}
