cmake_minimum_required(VERSION 3.9.1)

project(ONVIF_CLIENT)

set(CMAKE_CXX_STANDARD 14)

FILE(GLOB SOAP_DIR "SOAP")
message(STATUS "SOAP_DIR" ${SOAP_DIR})

FILE(GLOB SOAP_SRC_S "${SOAP_DIR}/*.cpp")
message(STATUS "${SOAP_SRC_S}")

find_package(Boost REQUIRED date_time regex)

add_executable(main main.cpp "${SOAP_SRC_S}")

target_link_libraries(main Boost::date_time Boost::regex)

include_directories(${Boost_INCLUDE_DIRS} "${SOAP_DIR}")


