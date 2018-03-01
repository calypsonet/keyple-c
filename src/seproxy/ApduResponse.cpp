#include "ApduResponse.hpp"
#include "osKplMem.h"

namespace keyple {

	namespace seproxy {

		ApduResponse::ApduResponse(uint8_t inApdu[257], uint16_t inApduLength, bool inSuccessful) 
		{
			OsKpl_Memset(apdu, 0, 257);
			OsKpl_Memset(data, 0, 255);
			OsKpl_Memset(status, 0, 2);
			setApdu(inApdu, inApduLength, inSuccessful);
		}

		ApduResponse::ApduResponse() {}

		ApduResponse::~ApduResponse() {}

		void ApduResponse::setApdu(uint8_t inApdu[257], uint16_t inApduLength, bool inSuccessful) {

			successful = inSuccessful;

			if(!inSuccessful) {
				return;
			}

			OsKpl_Memcpy(apdu, inApdu, inApduLength);
			apduLength = inApduLength;

			if(apduLength > 2) {
				dataLength = apduLength - 2;
				OsKpl_Memcpy(data, inApdu, dataLength);
			}

			OsKpl_Memcpy(status, inApdu + dataLength, 2);
		}

		uint8_t* ApduResponse::getData() {return data;}

		uint16_t ApduResponse::getDataLength() {return dataLength;}

		uint8_t* ApduResponse::getStatus() {return status;} // to replace returned type by a short?

		uint8_t* ApduResponse::getApdu() {return apdu;}

		uint16_t ApduResponse::getApduLength() {return apduLength;}

		bool ApduResponse::isSuccessful() {return successful;}

	}

}
