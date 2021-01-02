cmake_minimum_required(VERSION 3.5)

project(GUI_Util LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-video-1.0)

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
  FormVideoConfiguration.cpp
  FormVideoConfiguration.h
  FormVideoConfiguration.ui
  DialogWaiting.cpp
  DialogWaiting.h
  DialogWaiting.ui
  Resources.qrc
  FormVideoLive.cpp
  FormVideoLive.h
  FormVideoLive.ui
  streamhandler.cpp
  streamhandler.h
  playerwidget.ui
  playerwidget.cpp
  playerwidget.h
)

include_directories(
    "${ROOT_DIR}/Driver/Headers"
    "${GST_INCLUDE_DIRS}")

target_compile_options(GUI_Util PUBLIC "${GST_CFLAGS}")

target_link_directories(GUI_Util PUBLIC "C:/dev_libs/gstreamer/1.0/x86_64/lib")

target_link_libraries(GUI_Util PRIVATE
    Qt5::Widgets
    "${ROOT_DIR}/Driver/Builds/build_x64/Debug/libonvif.lib"
    "${GST_LIBRARIES}")

file(COPY
        "${ROOT_DIR}/Driver/Builds/build_x64/Debug/libonvif.dll"
        "${ROOT_DIR}/Libs/OpenSSL/bin/libeay32.dll"
        "${ROOT_DIR}/Libs/OpenSSL/bin/ssleay32.dll"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
