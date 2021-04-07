cmake_minimum_required(VERSION 3.5)

project(ONVIF_Driver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)

FILE(GLOB INTERFACES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces")
FILE(GLOB HDRS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Headers")
FILE(GLOB SRCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Sources")

FILE(GLOB INTERFACES "${INTERFACES_DIR}/*.h")
FILE(GLOB HDRS "${HDRS_DIR}/*.h")
FILE(GLOB SRCS "${SRCS_DIR}/*.cpp")

FILE(GLOB ReplayFiles
	"${CMAKE_CURRENT_SOURCE_DIR}/Replay/replay_control_impl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Replay/replay_control_impl.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Replay/replay_factory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Replay/replay_factory.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Replay/replay_search_impl.cpp"	
	"${CMAKE_CURRENT_SOURCE_DIR}/Replay/replay_search_impl.h"	
)

get_filename_component(PARENT_DIR "../" ABSOLUTE)

find_package(Boost REQUIRED system date_time regex)

IF(NOT Boost_FOUND)
	message(FATAL_ERROR "Not found Boost")
ELSE()
	message("+++ Boost INCLUDE_DIR is ${Boost_INCLUDE_DIRS}")
	include_directories(${Boost_INCLUDE_DIRS})
ENDIF()	

add_library(
	libonvif SHARED
	"${INTERFACES}"
	"${SRCS}"
	"${HDRS}"
	"${ReplayFiles}"
	"${SOAP_HDRS}"
)

source_group("Interfaces" FILES ${INTERFACES})
source_group("Replay" FILES ${ReplayFiles})

target_compile_options(libonvif PUBLIC "-DWITH_OPENSSL" "-DWITH_DOM")

set(OpenSSL_DIR "${PARENT_DIR}/Libs/OpenSSL/")
target_link_libraries(libonvif Boost::date_time Boost::regex
	"${PARENT_DIR}/Libs/GSoap_Lib.lib"
	"${OpenSSL_DIR}/lib/libeay32.lib"
	"${OpenSSL_DIR}/lib/ssleay32.lib"
)

include_directories(
	"${Boost_INCLUDE_DIR}"
	"${INTERFACES_DIR}"
	"${HDRS_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/Replay"
	"${CMAKE_CURRENT_SOURCE_DIR}/GSoap"	
	"${OpenSSL_DIR}/include"
)