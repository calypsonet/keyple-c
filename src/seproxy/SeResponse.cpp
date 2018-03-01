#include "SeResponse.hpp"
#include "osKplMem.h"

namespace keyple {
	namespace seproxy {

		SeResponse::SeResponse(bool inChannelPreviouslyOpen, ApduResponse* inFci, uint8_t inAtr[257], uint16_t inAtrLength) 			
		{
			OsKpl_Memset(atr, 0, 257);

			setData(inChannelPreviouslyOpen, inFci, inAtr, inAtrLength);
		}

		void SeResponse::setData(bool inChannelPreviouslyOpen, ApduResponse* inFci, uint8_t inAtr[257], uint16_t inAtrLength) 
		{
			channelPreviouslyOpen = inChannelPreviouslyOpen;			
			OsKpl_Memcpy(atr, inAtr, inAtrLength);
			atrLength = inAtrLength;
			fci.setApdu(inFci->getApdu(), inFci->getApduLength(), inFci->isSuccessful());
		}

		SeResponse::SeResponse() {}

		SeResponse::~SeResponse() {}

		bool SeResponse::wasChannelPreviouslyOpen() {return channelPreviouslyOpen;}

		ApduResponse* SeResponse::getFci() {return &fci;}

		std::list<ApduResponse>* SeResponse::getApduResponses() {return &apduResponses;}

		uint8_t* SeResponse::getAtr() {return atr;}

		uint16_t SeResponse::getAtrLength() {return atrLength;} 
	}
}