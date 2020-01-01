cmake_minimum_required(VERSION 3.5)

project(GUI_Util LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

get_filename_component(ROOT_DIR "../../" ABSOLUTE)

add_executable(GUI_Util
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  adddevicedialog.cpp
  adddevicedialog.h
  adddevicedialog.ui
  DevicesManager.cpp
  DevicesManager.h
  FormDeviceMaintenance.cpp
  FormDeviceMaintenance.h
  FormDeviceMaintenance.ui
)

include_directories("${ROOT_DIR}/Driver/Headers")
target_link_libraries(GUI_Util PRIVATE Qt5::Widgets "${ROOT_DIR}/Driver/Builds/Debug/libonvif.lib")

file(COPY
        "${ROOT_DIR}/Driver/Builds/Debug/libonvif.dll"
        "${ROOT_DIR}/Libs/OpenSSL/bin/libeay32.dll"
        "${ROOT_DIR}/Libs/OpenSSL/bin/ssleay32.dll"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
