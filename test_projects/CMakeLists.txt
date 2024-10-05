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

set(Boost_DEBUG 1)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.74.0 QUIET COMPONENTS serialization)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

cmake_minimum_required(VERSION 3.10)
project(StaticAnalysisTestSuite)

include(GoogleTest)

# Add subdirectories for each test project
add_subdirectory(projects)
add_subdirectory(utils)

add_executable(test_all test_all.cpp)

# Link with all the test libraries
target_link_libraries(test_all basic gtest gtest_main)
target_include_directories(test_all PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/projects/basic ${CMAKE_CURRENT_SOURCE_DIR}/projects/complex)

gtest_discover_tests(test_all basic_tests)