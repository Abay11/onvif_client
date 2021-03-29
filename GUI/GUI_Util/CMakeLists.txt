cmake_minimum_required(VERSION 3.5)

project(GUI_Util LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-video-1.0)

get_filename_component(ROOT_DIR "../../" ABSOLUTE)

set(GUI_FILES
mainwindow.ui
adddevicedialog.ui
FormDeviceMaintenance.ui
FormVideoConfiguration.ui
DialogWaiting.ui
FormVideoLive.ui
playerwidget.ui
recordingsform.ui
)

set(SRC_FILES
main.cpp
  mainwindow.cpp
  adddevicedialog.cpp
  DevicesManager.cpp
  FormDeviceMaintenance.cpp
  FormVideoConfiguration.cpp
  DialogWaiting.cpp
  FormVideoLive.cpp
  streamhandler.cpp
  playerwidget.cpp
  audioinputgrabber.cpp
  recordingsform.cpp
)

set(HDR_FILES
logging.h
mainwindow.h    
adddevicedialog.h    
DevicesManager.h  
FormDeviceMaintenance.h    
FormVideoConfiguration.h    
DialogWaiting.h  
Resources.qrc  
FormVideoLive.h    
streamhandler.h    
playerwidget.h  
audioinputgrabber.h
recordingsform.h
)

source_group(UI FILES ${GUI_FILES})
source_group(HDRS FILES ${HDR_FILES})
source_group(SRS FILES ${SRS_FILES})

add_executable(GUI_Util 
    ${GUI_FILES}
    ${SRC_FILES}
    ${HDR_FILES}
)


include_directories(
    "${ROOT_DIR}/Driver/Interfaces"
    "${ROOT_DIR}/Driver/Headers"
    "${ROOT_DIR}/Driver/Replay"
    "${GST_INCLUDE_DIRS}")

target_compile_options(GUI_Util PUBLIC "${GST_CFLAGS}")

target_link_directories(GUI_Util PUBLIC "C:/dev_libs/gstreamer/1.0/x86_64/lib")

target_link_libraries(GUI_Util PRIVATE
    Qt5::Widgets
    Qt5::Multimedia
    "${ROOT_DIR}/Driver/Builds/build_x64/Release/libonvif.lib"
    "${GST_LIBRARIES}")

file(COPY
        "${ROOT_DIR}/Driver/Builds/build_x64/Release/libonvif.dll"
        "${ROOT_DIR}/Libs/OpenSSL/bin/libeay32.dll"
        "${ROOT_DIR}/Libs/OpenSSL/bin/ssleay32.dll"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
