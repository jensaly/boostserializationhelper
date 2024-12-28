cmake_minimum_required(VERSION 3.10)

add_library(basic STATIC basic_class.hpp)

target_link_libraries(basic common ${Boost_LIBRARIES})

target_include_directories(basic SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/common)

set_target_properties(basic PROPERTIES LINKER_LANGUAGE CXX)