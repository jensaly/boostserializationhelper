cmake_minimum_required(VERSION 3.10)

add_library(complex STATIC complex_class.h test_project.cpp)

# For testing the correctness of this project (e.g., building/running separately)
#add_executable(run_project1_tests project1_test.cpp)
target_link_libraries(complex gtest gtest_main sh_lib)

target_include_directories(complex PUBLIC ${CMAKE_SOURCE_DIR}/src_lib)
#add_test(NAME Project1Test COMMAND run_project1_tests)