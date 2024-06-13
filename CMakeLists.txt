cmake_minimum_required(VERSION 3.22)
project(seminar)

set(CMAKE_CXX_STANDARD 11)

include_directories("src")
add_subdirectory("src")
add_subdirectory("test")

# Add submodules
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/rapidcheck ${CMAKE_BINARY_DIR}/lib/rapidcheck)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/rapidcheck/extras/gtest ${CMAKE_BINARY_DIR}/lib/rapidcheck/extras/gtest)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/googletest ${CMAKE_BINARY_DIR}/lib/googletest)

# Include the script to suppress warnings
include(${CMAKE_SOURCE_DIR}/suppress_warnings.cmake)

# Suppress warnings for RapidCheck
suppress_warnings(rapidcheck)
suppress_warnings(gtest)