cmake_minimum_required(VERSION 3.9)

project(Aup_ClientApp)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

include_directories(lib)

file(GLOB_RECURSE HEADERS "src/*.h")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} )
add_subdirectory(lib)

include_directories(${Qt5Widgets_INCLUDES})

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} auplib)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
