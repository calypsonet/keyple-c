#include "ApduRequest.hpp"
#include "osKplMem.h"

namespace keyple {
	namespace seproxy {

		ApduRequest::ApduRequest(uint8_t inApdu[260], uint16_t inApduLength, bool inCase4) 
		{
			OsKpl_Memset(apdu, 0, 260);
			OsKpl_Memset(data, 0, 260);

			CLA = inApdu[0];
			INS = inApdu[1];
			P1 = inApdu[2];
			P2 = inApdu[3];

			if(inApduLength == 5) {

				Le = inApdu[4];

			} else {

				Lc = inApdu[4];
				OsKpl_Memcpy(data, inApdu+5, Lc);

				if(inApduLength == (Lc + 6)) {
					Le = inApdu[inApduLength-1];
				} 
			}

			OsKpl_Memcpy(apdu, inApdu, inApduLength);
			apduLength = inApduLength;
			case4 = inCase4;
		}

		ApduRequest::~ApduRequest() {}

		uint8_t ApduRequest::getCLA() {return CLA;}

		uint8_t ApduRequest::getINS() {return INS;}

		uint8_t ApduRequest::getP1() {return P1;}

		uint8_t ApduRequest::getP2() {return P2;}

		uint8_t ApduRequest::getLc() {return Lc;}

		uint8_t ApduRequest::getLe() {return Le;}

		uint8_t* ApduRequest::getApdu() {return apdu;}

		uint16_t ApduRequest::getApduLength() {return apduLength;}

		bool ApduRequest::isCase4() {return case4;}

		uint8_t* ApduRequest::getData() {return data;}

	}
}