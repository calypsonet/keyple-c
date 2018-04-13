/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#define ATR_MIFARE_1K                                                         \
    "\x3B"                 /* Initial header */                               \
    "\x8F"                 /* T0 */                                           \
    "\x80"                 /* TD1 */                                          \
    "\x01"                 /* TD2 */                                          \
    "\x80"                 /* Category Indicator (TLV-COMPACT data object) */ \
    "\x4F"                 /* Tag Appl. Id. Precence Indicator */             \
    "\x0C"                 /* Tag Length */                                   \
    "\xA0\x00\x00\x03\x06" /* Registered app. provider identifier (RID) */    \
    "\x03"                 /* SS Standard */                                  \
    "\x00\x01"             /* NNNN Card name - Mifare 1k */                   \
    "\x00\x00\x00\x00"     /* r.f.u */                                        \
    "\x6A"                 /* TCK */

#define ATR_MIFARE_4K                                         \
    "\x3B"                 /* Initial */                      \
    "\x8F"                 /* T0 */                           \
    "\x80"                 /* TD1 */                          \
    "\x01"                 /* TD2 */                          \
    "\x80"                 /* Category Indicator */           \
    "\x4F"                 /* Appl. Id. Precence Indicator */ \
    "\x0C"                 /* Tag Length */                   \
    "\xA0\x00\x00\x03\x06" /* AID */                          \
    "\x03"                 /* SS */                           \
    "\x00\x02"             /* NN - Mifare 4k */               \
    "\x00\x00\x00\x00"     /* r.f.u */                        \
    "\x69"                 /* TCK */

#define ATR_MIFARE_UL                                         \
    "\x3B"                 /* Initial */                      \
    "\x8F"                 /* T0 */                           \
    "\x80"                 /* TD1 */                          \
    "\x01"                 /* TD2 */                          \
    "\x80"                 /* Category Indicator */           \
    "\x4F"                 /* Appl. Id. Precence Indicator */ \
    "\x0C"                 /* Tag Length */                   \
    "\xA0\x00\x00\x03\x06" /* AID */                          \
    "\x03"                 /* SS */                           \
    "\x00\x03"             /* NN - Mifare Ultra Light */      \
    "\x00\x00\x00\x00"     /* r.f.u */                        \
    "\x68"                 /* TCK */
