cmake_minimum_required(VERSION 3.9)

project(AutoUpdateProgress)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
#include_directories(${Qt5Widgets_INCLUDES})
#target_link_libraries(${PROJECT_NAME} Qt5::Widgets Urlmon.lib)
