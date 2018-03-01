#include "ReadersPlugin.hpp"
#include "seProxyMngr.h"
#include "osKplMem.h"
#include "osKplStr.h"

namespace keyple {

	namespace seproxy {

		ReadersPlugin::ReadersPlugin(void* inProxyContext, uint32_t inPluginType) {

			proxyContext = inProxyContext;

			ExecutionStatus status = getPlugin(inProxyContext, (T_PluginType) inPluginType, &pluginContext);

			if(status != EXECUTION_OK) {return;}

			int8_t plugName[100];

			status = getPluginName(pluginContext, plugName);
			if(status != EXECUTION_OK) {return;}
			pluginName = std::string((const char*) plugName);

			int8_t *readersMultiString = NULL;
			uint32_t readerMultiStringSize = 0;

			status = getReaderDevNames(pluginContext, readersMultiString, &readerMultiStringSize);
			if(status != EXECUTION_OK) {return;}

			readersMultiString = (int8_t*) OsKpl_Malloc(readerMultiStringSize);
			if(readersMultiString == NULL) {return;}

			status = getReaderDevNames(pluginContext, readersMultiString, &readerMultiStringSize);
			if(status != EXECUTION_OK) {return;}

			proxyReaderList.clear();

			for(uint32_t iterReaders = 0; iterReaders < readerMultiStringSize; iterReaders++) {

				if(OsKpl_Strlen(readersMultiString+iterReaders) > 1) {
					ProxyReader reader(std::string((const char*) readersMultiString+iterReaders), pluginContext);
					proxyReaderList.push_back(reader);
				}
				iterReaders += OsKpl_Strlen(readersMultiString+iterReaders);
			}

			OsKpl_Free(readersMultiString);
		}

		ReadersPlugin::~ReadersPlugin() {}

		std::string ReadersPlugin::getName() {return pluginName;}

		std::list<ProxyReader>* ReadersPlugin::getReaders() {return &proxyReaderList;}

	}
}
