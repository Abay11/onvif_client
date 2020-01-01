cmake_minimum_required(VERSION 3.5)

project(ONVIF_Driver)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)

FILE(GLOB HDRS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Headers")
FILE(GLOB SRCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Sources")

FILE(GLOB GSoap_DIR "${CMAKE_CURRENT_SOURCE_DIR}/GSoap")

FILE(GLOB SOAP_SRCS	
	"${GSoap_DIR}/*.cpp"
	"${GSoap_DIR}/*.c"
)

FILE(GLOB SOAP_HDRS
	"${GSoap_DIR}/*.h"
	"${GSoap_DIR}/*.nsmap"	
)

FILE(GLOB HDRS "${HDRS_DIR}/*.h")
FILE(GLOB SRCS "${SRCS_DIR}/*.cpp")

get_filename_component(PARENT_DIR "../" ABSOLUTE)
set(Boost_INCLUDE_DIR "${PARENT_DIR}/Libs/Boost/include")

find_package(Boost REQUIRED date_time regex)

add_library(
	libonvif SHARED
	"${SRCS}"
	"${HDRS}"	
	"${SOAP_SRCS}"
	"${SOAP_HDRS}"
)

source_group("GSoap" FILES ${SOAP_HDRS} ${SOAP_SRCS})
#source_group("Headers" FILES ${Headers})

target_compile_options(libonvif PUBLIC "-DWITH_OPENSSL" "-DWITH_DOM")

set(OpenSSL_DIR "${PARENT_DIR}/Libs/OpenSSL/")
target_link_libraries(libonvif Boost::date_time Boost::regex
	"${OpenSSL_DIR}/lib/libeay32.lib"
	"${OpenSSL_DIR}/lib/ssleay32.lib"
)

include_directories(
	"${Boost_INCLUDE_DIR}"
	"${HDRS_DIR}"
	"${GSoap_DIR}"	
	"${OpenSSL_DIR}/include"	
)