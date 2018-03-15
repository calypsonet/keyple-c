#include <stdio.h>

#include "keyple_reader_interface.h"
#include "pcsc_readers.h"

#define PO_READER_NAME "ASK"
#define CSM_READER_NAME "Identive"

#define PO_READER_INDEX 0
#define CSM_READER_INDEX 1

#define POOL_INDEX_0 0

#define CLA 0
#define INS 1
#define P1 2
#define P2 3
#define P3 4

static int doTransaction(void)
{
    uint8_t pool[1024]                           = {0};
    struct requestParameters_s requestParameters = {0};
    struct apduCommand_s *command1, *command2, *command3;
    struct apduResponse_s *response1, *response2, *response3;

    provideMemoryPool(POOL_INDEX_0, pool, sizeof(pool));

    // APDUs allocations
    if (linkNewApdu(POOL_INDEX_0, &command1, NULL, 5, &response1, NULL, 256) != EXECUTION_OK)
    {
        return -1;
    }
    if (linkNewApdu(POOL_INDEX_0, &command2, &command1, 5, &response2, &response1, 256) != EXECUTION_OK)
    {
        return -1;
    }
    if (linkNewApdu(POOL_INDEX_0, &command3, &command2, 5, &response3, &response2, 256) != EXECUTION_OK)
    {
        return -1;
    }

    command1->data[CLA] = 0x00;
    command1->data[INS] = 0xB0;
    command1->data[P1]  = 0x00;
    command1->data[P2]  = 0x00;
    command1->data[P3]  = 0x00;
    command1->len       = 5;

    command2->data[CLA] = 0x00;
    command2->data[INS] = 0xD0;
    command2->data[P1]  = 0x00;
    command2->data[P2]  = 0x00;
    command2->data[P3]  = 0x00;
    command2->len       = 5;

    command3->data[CLA] = 0x00;
    command3->data[INS] = 0x84;
    command3->data[P1]  = 0x00;
    command3->data[P2]  = 0x00;
    command3->data[P3]  = 0x08;
    command3->len       = 5;

    executeRequest(PO_READER_INDEX, requestParameters, command1, response1);

    return 0;
}

int main(void)
{
    bool allReadersFound = false;
    char readerNames[NBR_READERS][READER_NAME_MAX_LENGTH];

    if (initPcsc(readerNames[0], readerNames[1]) == KPL_RDR_SUCCESS)
    {
        if (strstr(readerNames[0], PO_READER_NAME) != NULL)
        {
            assignReader(PO_READER_INDEX, readerNames[0]);
            if (strstr(readerNames[1], CSM_READER_NAME) != NULL)
            {
                assignReader(CSM_READER_INDEX, readerNames[1]);
                allReadersFound = true;
            }
        }
        else
        {
            if (strstr(readerNames[1], PO_READER_NAME) != NULL)
            {
                assignReader(PO_READER_INDEX, readerNames[1]);
                if (strstr(readerNames[0], CSM_READER_NAME) != NULL)
                {
                    assignReader(CSM_READER_INDEX, readerNames[0]);
                    allReadersFound = true;
                }
            }
        }
        if (allReadersFound)
        {
            struct readerDescriptor_s reader;

            // set SDK reader callbacks for PO reader (0)
            reader.openChannel  = reader0_openChannel;
            reader.closeChannel = reader0_closeChannel;
            reader.transmit     = reader0_transmit;

            setReader(PO_READER_INDEX, reader);

            // set SDK reader callbacks for CSM reader (1)
            reader.openChannel  = reader1_openChannel;
            reader.closeChannel = reader1_closeChannel;
            reader.transmit     = reader1_transmit;

            setReader(CSM_READER_INDEX, reader);

            doTransaction();
        }
    }
    printf("Press a key to exit.");
    getchar();
    return 0;
}
