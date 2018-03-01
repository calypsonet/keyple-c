#include "SeRequest.hpp"
#include "ApduRequest.hpp"
#include "osKplMem.h"

namespace keyple {
	namespace seproxy {

		SeRequest::SeRequest(
			uint8_t inAidToSelect[16], 
			uint16_t inAidToSelectLength, 
			std::list<ApduRequest>* inApduRequests, 
			bool inKeepChannelOpen) 
		{
			if(inAidToSelectLength > 0) {
				OsKpl_Memcpy(aidToSelect, inAidToSelect, inAidToSelectLength);
			}
			aidToSelectLength = inAidToSelectLength;
			apduRequests = inApduRequests;
			keepChannelOpen = inKeepChannelOpen;
		}

		SeRequest::~SeRequest() {}

		uint8_t* SeRequest::getAidToSelect() {return aidToSelect;}

		uint16_t SeRequest::getAidToSelectLength() {return aidToSelectLength;}

		std::list<ApduRequest>* SeRequest::getApduRequests() {return apduRequests;}

		bool SeRequest::askKeepChannelOpen() {return keepChannelOpen;}

	}
}