cmake_minimum_required(VERSION 3.10)

# Create a static library for the project
add_library(basic STATIC basic_class.h)

# Add test files as part of this project
add_library(basic_tests STATIC test_project.cpp)

target_include_directories(basic PUBLIC ${CMAKE_SOURCE_DIR}/src_lib)
target_include_directories(basic_tests PUBLIC ${CMAKE_SOURCE_DIR}/src_lib)

# Link tests to GTest and the project library
target_link_libraries(basic basic_tests gtest gtest_main sh_lib)