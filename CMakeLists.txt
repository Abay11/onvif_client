cmake_minimum_required(VERSION 3.9.1)

project(TestingProjectName)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED date_time regex)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(main main.cpp)
target_link_libraries(main Boost::date_time Boost::regex)