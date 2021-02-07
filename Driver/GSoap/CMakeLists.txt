cmake_minimum_required(VERSION 3.5)

project(GSoap_Lib)

set(CMAKE_CXX_STANDARD 14)

FILE(GLOB GSoap_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

FILE(GLOB SOAP_SRCS	
	"${GSoap_DIR}/*.cpp"
	"${GSoap_DIR}/*.c"
)

FILE(GLOB SOAP_HDRS
	"${GSoap_DIR}/*.h"
	"${GSoap_DIR}/*.nsmap"	
)

add_library(
	GSoap_Lib STATIC	
	"${SOAP_SRCS}"
	"${SOAP_HDRS}"
)

source_group("GSoap" FILES ${SOAP_HDRS} ${SOAP_SRCS})

target_compile_options(GSoap_Lib PUBLIC "-DWITH_OPENSSL" "-DWITH_DOM")

set(OpenSSL_DIR "${PARENT_DIR}/Libs/OpenSSL/")
# target_link_libraries(GSoap_Lib
# 	"${OpenSSL_DIR}/lib/libeay32.lib"
# 	"${OpenSSL_DIR}/lib/ssleay32.lib"
# )

# include_directories(
# 	"${OpenSSL_DIR}/include"	
# )