cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(test_utils STATIC test_utils.h test_utils.cpp)

target_include_directories(test_utils PUBLIC ${CMAKE_SOURCE_DIR})