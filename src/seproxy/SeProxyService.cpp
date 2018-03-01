#include "SeProxyService.hpp"
#include "seProxyMngr.h"
#include "osKplMem.h"

namespace keyple {

	namespace seproxy {

		SeProxyService* SeProxyService::getInstance() {

			static SeProxyService* uniqueInstance = 0;

			if(!uniqueInstance) {
				uniqueInstance = new SeProxyService();
			}

			return uniqueInstance;
		}

		uint16_t SeProxyService::getVersion() {return version;}

		void SeProxyService::setPlugins(std::list<ReadersPlugin>* inPlugins) {}

		std::list<ReadersPlugin>* SeProxyService::getPlugins() {
			return &plugins;
		}

		SeProxyService::SeProxyService() {
			ExecutionStatus status;
			uint16_t numberOfPlugins = 0;
			T_PluginType *pluginList = NULL;

			status = initProxyManager(K_DEBUG_MODE_ON, (int8_t*) "", &version, &proxyContext);
			if(status != EXECUTION_OK) {return;}

			status = getSupportedPlugins(proxyContext, pluginList, &numberOfPlugins);
			if(status != EXECUTION_OK) {return;}

			pluginList = (T_PluginType*) OsKpl_Malloc(sizeof(T_PluginType) * numberOfPlugins);

			status = getSupportedPlugins(proxyContext, pluginList, &numberOfPlugins);
			if(status != EXECUTION_OK) {return;}

			plugins.clear();

			for(uint32_t i = 0; i < numberOfPlugins; i++) 
			{
				ReadersPlugin plugin(proxyContext, pluginList[i]);
				plugins.push_back(plugin);
			}
		}
	
		SeProxyService::~SeProxyService() {
			terminateProxyManager(proxyContext);
		}
	}
}
