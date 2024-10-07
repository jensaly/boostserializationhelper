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

include(GoogleTest)

cmake_minimum_required(VERSION 3.10)

# Create a static library for the project
add_executable(test_main main.cpp basic.h)

# Link tests to GTest and the project library
target_link_libraries(test_main basic sh_lib gtest gtest_main sh_lib)

target_include_directories(test_main PUBLIC ${CMAKE_SOURCE_DIR}/projects ${CMAKE_SOURCE_DIR}/src_lib)

gtest_discover_tests(test_main)

file(COPY ${CMAKE_SOURCE_DIR}/projects DESTINATION ${CMAKE_INSTALL_PREFIX}/projects)
