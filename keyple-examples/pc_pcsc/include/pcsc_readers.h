#ifndef PCSC_READERS_H
#define PCSC_READERS_H

#include <stdbool.h>
#include <stdint.h>
#include <winscard.h>
#ifdef __linux__
#include <reader.h>
#endif  // __linux__

// Constants
#define READER_NAME_MAX_LENGTH 256
#define NBR_READERS 2

// Functions
uint16_t initPcsc(char *reader0Name, char *reader1Name);
uint16_t assignReader(uint32_t readerIndex, char *readerName);

uint16_t reader0_openChannel(struct cardInfo_s *outCardInfo);
uint16_t reader0_closeChannel(uint8_t inDisposition);
uint16_t reader0_transmit(uint8_t *inCommand, uint16_t inCommandLength,
                          uint8_t *outResponse, uint16_t *inOutResponseLength);

uint16_t reader1_openChannel(struct cardInfo_s *outCardInfo);
uint16_t reader1_closeChannel(uint8_t inDisposition);
uint16_t reader1_transmit(uint8_t *inCommand, uint16_t inCommandLength,
                          uint8_t *outResponse, uint16_t *inOutResponseLength);

#endif  // PCSC_READERS_H
