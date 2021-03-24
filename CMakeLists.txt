cmake_minimum_required(VERSION 3.17)
project(pbrain-Incephalon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB SOURCES sources/*.cpp)
include_directories("./includes/")


add_executable(pbrain-Incephalon main.cpp sources/Incephalon.cpp
        sources/BoardInstance.cpp
        includes/Commands.h
        sources/Commands.cpp
        includes/SharedFunctions.h
        sources/SharedFunctions.cpp
        sources/Settings.cpp
        includes/Structures.h
        includes/Settings.h)