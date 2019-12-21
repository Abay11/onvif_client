cmake_minimum_required(VERSION 3.9.1)

project(ONVIF_CLIENT)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)

FILE(GLOB HDRS_DIR "Headers")
FILE(GLOB SOAP_HDRS_DIR "Headers/SOAP")

FILE(GLOB SOAP_SRCS "Sources/SOAP/*")
FILE(GLOB SOAP_HDRS "Headers/SOAP/*")
FILE(GLOB HDRS "Headers/*.h")
FILE(GLOB SRCS "Sources/*.cpp")

set(Boost_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libs/Boost/include")

find_package(Boost REQUIRED date_time regex)

add_library(
	libonvif SHARED
	"${SRCS}"
	"${HDRS}"	
	"${SOAP_SRCS}"
	"${SOAP_HDRS}"	
	)

target_compile_options(libonvif PUBLIC "-DWITH_OPENSSL" "-DWITH_DOM")

target_link_libraries(libonvif Boost::date_time Boost::regex
	"${CMAKE_CURRENT_SOURCE_DIR}/Libs/OpenSSL/lib/libeay32.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/Libs/OpenSSL/lib/ssleay32.lib")

file(COPY
	${CMAKE_CURRENT_SOURCE_DIR}/Libs/OpenSSL/bin/libeay32.dll
	${CMAKE_CURRENT_SOURCE_DIR}/Libs/OpenSSL/bin/ssleay32.dll
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)

file(COPY
	${CMAKE_CURRENT_SOURCE_DIR}/Libs/OpenSSL/bin/libeay32.dll
	${CMAKE_CURRENT_SOURCE_DIR}/Libs/OpenSSL/bin/ssleay32.dll
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
	
include_directories(
	${Boost_INCLUDE_DIRS}
	"${HDRS_DIR}"
	"${SOAP_HDRS_DIR}"	
	"Libs/OpenSSL/include"	
)


