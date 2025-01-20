cmake_minimum_required(VERSION 3.10)

add_library(split STATIC split_ampersand.hpp split_directional.hpp)

target_link_libraries(split gtest gtest_main sh_lib)
