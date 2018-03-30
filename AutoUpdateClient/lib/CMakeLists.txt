cmake_minimum_required(VERSION 3.9)

project(auplib)

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

add_library(${PROJECT_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER autoupdater.h)

#install(TARGETS ${PROJECT_NAME} DESTINATION lib)
#file(GLOB HEADERS *.h)
#install(FILES ${HEADERS} DESTINATION lib)
