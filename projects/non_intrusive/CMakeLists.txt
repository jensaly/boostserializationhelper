cmake_minimum_required(VERSION 3.10)

add_library(non_intrusive STATIC non_intrusive.hpp)

target_link_libraries(non_intrusive common ${Boost_LIBRARIES})

target_include_directories(non_intrusive SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/common)

set_target_properties(non_intrusive PROPERTIES LINKER_LANGUAGE CXX)