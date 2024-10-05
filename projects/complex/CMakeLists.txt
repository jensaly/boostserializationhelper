cmake_minimum_required(VERSION 3.10)

add_library(complex STATIC complex_class.h)

target_link_libraries(complex gtest gtest_main sh_lib)
