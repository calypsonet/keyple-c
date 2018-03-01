// --------------------------------------------------------
// Code generated by Papyrus C++
// --------------------------------------------------------

#ifndef KEYPLE_SEPROXY_PROXYREADER_H
#define KEYPLE_SEPROXY_PROXYREADER_H

/************************************************************
 ProxyReader class header
 ************************************************************/

#include <map>
#include <string>
#include <errno.h>

#include "ExecutionStatus.hpp"
#include "SeRequest.hpp"
#include "SeResponse.hpp"


namespace keyple {

	namespace seproxy {

		/************************************************************/
		/**
		 * Abstract class extended by each plugins of readers’ drivers.
		 */
		class ProxyReader {
		public:

			ProxyReader(std::string inReaderName, void * inPluginContext);

			~ProxyReader();

			/**
 			 * 
			 * @return name 
			 */
			std::string getName();

			/**
			 * 
			 * @param seApplicationRequests 
			 * @param seApplicationResponses
			 * @return the status of the command execution 
			 *
			 * Manages:
			 * 		ChannelStateReaderError
			 * 		InvalidApduReaderError
			 * 		IOReaderError
			 * 		TimeoutReaderError
			 * 		UnexpectedReaderError
			 *
			 */
			ExecutionStatus transmit(SeRequest* inSeApplicationRequest, SeResponse* outSeApplicationResponse);

			/**
			 * 
			 * @return the SE presence 
			 */
			bool isSePresent();

			/**
			 * 
			 * @return parameters/settings 
			 */
			std::map<std::string, std::string>* getParameters();

			/**
			 * 
			 * @param key 
			 * @param value 
			 * @param response 
			 * @return the status of the execution of the command
			 *
			 * Manages:
			 * 		WrongParameterKeyError
			 * 		InconsistentParameterValueError
			 * 		ParameterProcessingErrorError
			 *
			 */
			ExecutionStatus setAParameter(std::string inKey, std::string inValue);

			/**
			 * Set proprietary settings for a plugin
			 * @param settings 
			 * @return the status of the execution of the command 
			 *
			 * 	 * Manages:
			 * 		WrongParameterKeyError
			 * 		InconsistentParameterValueError
			 * 		ParameterProcessingError
			 *
			 */
			ExecutionStatus setParameters(std::map<std::string, std::string>* inSettings);

		private:

			void *proxyReaderContext;

			std::string readerName;

			void *pluginContext;

			bool readerInitialized = false;

			std::map<std::string, std::string> configurationMap;
		};
		/************************************************************/
		/* External declarations (package visibility)               */
		/************************************************************/

		/* Inline functions                                         */

	} // of namespace seproxy

} // of namespace keyple

/************************************************************
 End of ProxyReader class header
 ************************************************************/

#endif
