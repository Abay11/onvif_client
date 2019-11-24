cmake_minimum_required(VERSION 3.9.1)

project(ONVIF_CLIENT)

set(CMAKE_CXX_STANDARD 14)

FILE(GLOB HDRS_DIR "Headers")
FILE(GLOB SOAP_HDRS_DIR "Headers/SOAP")

FILE(GLOB SOAP_SRCS "Sources/SOAP/*.cpp")
FILE(GLOB SOAP_HDRS "Headers/SOAP/*.h")
FILE(GLOB HDRS "Headers/*.h")
FILE(GLOB SRCS "Sources/*.cpp")

find_package(Boost REQUIRED date_time regex)

add_executable(
	main
	main.cpp
	"${SRCS}"
	"${HDRS}"	
	"${SOAP_SRCS}"
	"${SOAP_HDRS}")

target_link_libraries(main Boost::date_time Boost::regex)

include_directories(${Boost_INCLUDE_DIRS} "${HDRS_DIR}" "${SOAP_HDRS_DIR}")


