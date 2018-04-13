/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#ifndef READER_H
#define READER_H

#define CLOSE_TYPE_NONE 0
#define CLOSE_TYPE_RESET 1
#define CLOSE_TYPE_POWEROFF 2

#define CARDINFO_TAG_CARDPROTO 0xA0
#define CARDINFO_TAG_CARDTYPE 0xA1

/* More tags to be added */

/* Card protocols */
enum card_proto_e
{
    CARD_PROTO_T1,
    CARD_PROTO_T0,
    CARD_TYPE_ISO14443_A,
    CARD_TYPE_ISO14443_B,
    CARD_TYPE_ISO14443_F,
    CARD_TYPE_ISO18093,
};

/* card types */
enum card_type_e
{
    CARD_TYPE_CONTACT,
    CARD_TYPE_MIFARE_1K,
    CARD_TYPE_MIFARE_UL,
};

struct reader_s;

struct reader_ops_s
{
    /* Open an exclusive channel to the card.
     * For a contact card, the atr array is filled from the card.
     * For other cards, a minimal ATR can be returned, and more info is available via the
     * getcardinfo() function.
     * Returns zero if OK or negative if error.
     */
    int (*openchannel)(struct reader_s *rd, uint8_t atr[32], uint16_t *atrlen);

    /* Close the previously opened channel and release reader resources related to this channel.
     * Close_type can be used to use a specific behaviour after the channel is closed.
     */
    int (*closechannel)(struct reader_s *rd, uint8_t close_type);

    /* Get info about the current card in BER-TLV format.
     * Pointer is valid until close() is called.
     * Returns null of none.
     */
    uint8_t *(*getcardinfo)(struct reader_s *rd);

    /* Exchange ALL apdus in the request.
     * Return negative if error, or positive length
     */
    int (*transmitrequest)(struct reader_s *rd, struct serequest_params_s *req);

    /* Transmit ONE APDU.
     * Return negative if error, or positive length
     */
    int (*transmit)(struct reader_s *rd, uint8_t *cmd, uint16_t cmdlen, uint8_t *rsp, uint16_t rspmax);
};

struct reader_s
{
    struct reader_ops_s *ops; /* Pointer to device operations */
    bool opened;              /* Reader status */
    uint16_t ident;           /* Reader identifier */
};

#endif //READER_H
