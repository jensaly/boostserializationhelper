cmake_minimum_required(VERSION 3.10)

add_library(basic STATIC basic_class.h)

target_link_libraries(basic common ${Boost_LIBRARIES})

target_include_directories(basic PUBLIC ${CMAKE_SOURCE_DIR}/common)

set_target_properties(basic PROPERTIES LINKER_LANGUAGE CXX)