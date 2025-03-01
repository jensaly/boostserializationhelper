enable_testing()

# This solution is provided by Google. Apparently the most flexible way of getting GTest.
# https://github.com/google/googletest/blob/main/googletest/README.md
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

cmake_minimum_required(VERSION 3.10)

# Create a static library for the project
add_library(basic STATIC basic_class.h)

# Add test files as part of this project
add_library(basic_tests STATIC test_project.cpp)

target_include_directories(basic SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src_lib)
target_include_directories(basic_tests SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src_lib ${CMAKE_SOURCE_DIR}/test_projects/utils)

# Link tests to GTest and the project library
target_link_libraries(basic basic_tests gtest gtest_main sh_lib test_utils ${Boost_LIBRARIES})

gtest_discover_tests(basic
DISCOVERY_MODE PRE_TEST
)
