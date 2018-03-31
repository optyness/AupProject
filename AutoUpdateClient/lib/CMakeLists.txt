cmake_minimum_required(VERSION 3.9)

project(auplib)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
#include (GenerateExportHeader)
# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)

file(GLOB_RECURSE DLL_HEADERS "*.h")
#set (LIB_HEADERS
#       autoupdater.h
#       )

file(GLOB_RECURSE DLL_SOURCES "*.cpp")
#set (LIB_SOURCES
#        autoupdater.cpp
#        )

add_library(${PROJECT_NAME} SHARED ${DLL_HEADERS} ${DLL_SOURCES})

add_definitions(-DAUPLIB_LIBRARY)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER autoupdater.h)

#GENERATE_EXPORT_HEADER(${PROJECT_NAME}
#             BASE_NAME ${PROJECT_NAME}
#             EXPORT_MACRO_NAME ${PROJECT_NAME}_EXPORT
#             EXPORT_FILE_NAME ${PROJECT_NAME}_Export.h
#             STATIC_DEFINE ${PROJECT_NAME}_BUILT_AS_STATIC
#)

#install(TARGETS ${PROJECT_NAME} DESTINATION lib)
#file(GLOB HEADERS *.h)
#install(FILES ${HEADERS} DESTINATION lib)
