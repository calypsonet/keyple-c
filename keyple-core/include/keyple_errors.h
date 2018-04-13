/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

enum kpl_errors_e
{
    // general errors
    KPL_SUCCESS            = 0,
    KPL_ERROR_NULL_POINTER = -1,
    // context errors
    KPL_MAX_ALLOWED_READERS_NUMBER_REACHED = -1000,
    // reader errors
    KPL_RDR_INIT_ERROR               = -2000,
    KPL_RDR_CARD_INFO_ERROR          = -2001,
    KPL_RDR_CARD_CONNECTION_ERROR    = -2002,
    KPL_RDR_CARD_DISCONNECTION_ERROR = -2003,
    KPL_RDR_CARD_TRANSMIT_ERROR      = -2004
};
