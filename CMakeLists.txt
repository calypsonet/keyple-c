PROJECT(keyple C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "libkeyple")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

ADD_DEFINITIONS(-DK_OS_KPL_PLATFORM=K_OS_KPL_PLATFORM_WIN32 -DK_KPL_PCSC_PLUGIN_SUPPORT -DK_OS_KPL_MEMORY_DEBUG -D_CRT_SECURE_NO_WARNINGS)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy ${CMAKE_CURRENT_SOURCE_DIR}/src/pcscplugin) 

# SeProxyMngr library
add_library(SeProxyMngr SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/seProxyMngr.c 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/pluginMngr.c 
	${CMAKE_CURRENT_SOURCE_DIR}/src/pcscplugin/pcscPlgn.c 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/osKplMem.c 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/osKplStr.c 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/seProxyMngr.def 
)

IF(WIN32)
target_link_libraries(SeProxyMngr winscard)
ENDIF(WIN32)

# add the executable
add_executable (SeProxyTester 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/ApduResponse.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/ApduRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/SeRequest.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/SeResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/ProxyReader.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/ReadersPlugin.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/SeProxyService.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/terminal/SeProxyTester.cpp 
)

target_link_libraries(SeProxyTester SeProxyMngr)

