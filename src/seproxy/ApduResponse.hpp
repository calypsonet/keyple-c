// --------------------------------------------------------
// Code generated by Papyrus C++
// --------------------------------------------------------

#ifndef KEYPLE_SEPROXY_APDURESPONSE_H
#define KEYPLE_SEPROXY_APDURESPONSE_H

/************************************************************
 ApduResponse class header
 ************************************************************/

//#include "keyple/seproxy/Pkg_seproxy.h"
#include <stdint.h>

namespace keyple {
	namespace seproxy {

		/************************************************************/
		/**
		 * Class to define the elements of a single APDU command response
		 */
		class ApduResponse {
		public:

			/**
			 * Constructor
			 * Build the APDU response
			 * @param bytes 
			 */
			ApduResponse(uint8_t inApdu[257], uint16_t inApduLength, bool inSuccessful); 

			ApduResponse();

			/**
			 * Destructor
			 */
			~ApduResponse();

			/**
			 * 
			 * @return data(only dataout) 
			 */
			uint8_t* getData();

			uint16_t getDataLength();

			/**
			 * 
			 * @return the status of the command transmission 
			 */
			uint8_t* getStatus(); // to replace returned type by a short?

			/**
			 * 
			 * @return the bytes of the APDU response. 
			 */
			uint8_t* getApdu();


			uint16_t getApduLength();

			/**
			 * 
			 * @return status 
			 */
			bool isSuccessful();

			void setApdu(uint8_t inApdu[257], uint16_t inApduLength, bool inSuccessful);

		private:

			/**
			 * an array of the bytes of an APDU response(only dataout field)
			 */
			uint8_t data[255]; 

			uint16_t dataLength = 0;

			/**
			 * an array of the bytes of an APDU response (none structured, including the dataOut field and the status of the command).
			 */
			uint8_t apdu[257]; 

			uint16_t apduLength = 0;

			/**
			 * the status of the command
			 */
			uint8_t status[2];

			/**
			 * the success result of the processed APDU command.
			 */
			bool successful;
		};
	/************************************************************/
	/* External declarations (package visibility)               */
	/************************************************************/

	/* Inline functions                                         */

	} // of namespace seproxy
} // of namespace keyple

/************************************************************
 End of ApduResponse class header
 ************************************************************/

#endif
