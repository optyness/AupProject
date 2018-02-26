cmake_minimum_required(VERSION 3.9)

project(Aup_Lib)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

file(GLOB_RECURSE LIB_HEADERS "*.h")
#set (LIB_HEADERS
#       autoupdater.h
#       )

file(GLOB_RECURSE LIB_SOURCES "*.cpp")
#set (LIB_SOURCES
#        autoupdater.cpp
#        )

add_library(auplib STATIC ${LIB_HEADERS} ${LIB_SOURCES})
#target_link_libraries(auplib Qt5::Widgets)

#include_directories(${Qt5Widgets_INCLUDES})
