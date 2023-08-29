cmake_minimum_required(VERSION 3.5)

project(encedit VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
            args.h
            args.cpp
            main.cpp
            mainwindow.cpp
            mainwindow.h
            io.cpp
            io.h
            mweventhandler.h
            mweventhandler.cpp
            indexmodel.cpp
            indexmodel.h
            mainwindow.ui
            aes.h
            aes.cpp
)

set(app_icon_resource_windows "")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux") 
    MESSAGE(STATUS "current platform: Linux ") 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows") 
    MESSAGE(STATUS "current platform: Windows")
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin") 
    MESSAGE(STATUS "current platform: Mac OS X") 
ELSE () 
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}") 
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux") 

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(encedit
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET encedit APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(encedit SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(encedit
            ${PROJECT_SOURCES}
            ${app_icon_resource_windows}
        )
    endif()
endif()

target_link_libraries(encedit PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(encedit PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER encedit.xsy
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET encedit PROPERTY WIN32_EXECUTABLE true)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(encedit)
endif()
