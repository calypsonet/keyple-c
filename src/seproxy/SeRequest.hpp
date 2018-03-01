// --------------------------------------------------------
// Code generated by Papyrus C++
// --------------------------------------------------------

#ifndef KEYPLE_SEPROXY_SEREQUEST_H
#define KEYPLE_SEPROXY_SEREQUEST_H

/************************************************************
 SeRequest class header
 ************************************************************/

//#include "keyplesdk/seproxy/Pkg_seproxy.h"

#include <list>
#include <stdint.h>

#include "ApduRequest.hpp"

namespace keyple {
	namespace seproxy {

		/************************************************************/
		/**
		 * Class SeRequest to aggregate the elements of a request to a local or remote SE Reader
		 */
		class SeRequest {
		public:

			/**
			 * Constructor
			 * @param aidToSelect 
			 * @param apduRequests 
			 * @param keepChannelOpen 
			 */
			SeRequest(uint8_t inAidToSelect[16], uint16_t inAidToSelectLength, std::list<ApduRequest>* inApduRequests, bool inKeepChannelOpen);
			/**
			 * Destructor
			 */
			~SeRequest();

			/**
			 * 
			 * @return the current AID set to select 
			 */
			uint8_t* getAidToSelect();

			/**
			 *
			 * @return the length of the current AID set to select 
			 */
			uint16_t getAidToSelectLength();

			/**
			 * 
			 * @return the group of APDUs to be transmitted to the SE application for this instance of SeRequest
			 */
			std::list<ApduRequest>* getApduRequests();

			/**
			 * 
			 * @return whether or not to keep the channel open after exchanging the APDU requests with the SE
			 */
			bool askKeepChannelOpen();

		private:
			/**
			 * 
			 */
			uint8_t aidToSelect[16];

			/**
			 * 
			 */
			uint16_t aidToSelectLength;

			/**
			 * 
			 */
			bool keepChannelOpen;
			/**
			 * 
			 */
			std::list<ApduRequest> *apduRequests;
		};
/************************************************************/
/* External declarations (package visibility)               */
/************************************************************/

/* Inline functions                                         */

	} // of namespace seproxy
} // of namespace keyplesdk

/************************************************************
 End of SeRequest class header
 ************************************************************/

#endif
