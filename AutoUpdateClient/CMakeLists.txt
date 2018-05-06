cmake_minimum_required(VERSION 3.9)

project(AutoUpdateClient)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Network)

#include_directories("C:/Qt/Qt_project/build-AutoUpdatedll-Desktop_Qt_5_9_1_MSVC2015_64bit-Default")
#include_directories("C:/Qt/Qt_project/AutoUpdaterProject/AutoUpdatedll")
#link_directories(servicelib)

file(GLOB_RECURSE HEADERS "src/*.h")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} )
add_subdirectory(AutoUpdatedll)
add_subdirectory(AutoUpdateService)

include_directories(${Qt5Widgets_INCLUDES})

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} auplib)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Network)
