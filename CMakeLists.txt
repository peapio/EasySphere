cmake_minimum_required(VERSION 3.0)

# set(CMAKE_CXX_STANDARD 14)

project(esphere)

set(CMAKE_BUILD_TYPE Debug)
message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)