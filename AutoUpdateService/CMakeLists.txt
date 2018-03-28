cmake_minimum_required(VERSION 2.8)

project(AutoUpdateService)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

add_executable(${PROJECT_NAME} "main.cpp")

include_directories(${Qt5Widgets_INCLUDES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Urlmon.lib)
