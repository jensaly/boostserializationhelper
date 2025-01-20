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
project(test_projects)

include(GoogleTest)

add_subdirectory(utils)

add_executable(test_all test_all.cpp test_basic_project.cpp test_complex_project.cpp test_non_intrusive.cpp test_split_ampersand.cpp test_split_directional.cpp)

target_link_libraries(test_all basic test_utils gtest gtest_main ${Boost_LIBRARIES})
target_include_directories(test_all SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/projects/basic ${CMAKE_SOURCE_DIR}/projects/complex ${CMAKE_SOURCE_DIR}/projects/non_intrusive ${CMAKE_SOURCE_DIR}/projects/split utils)

gtest_discover_tests(test_all)

install(TARGETS test_all DESTINATION ${CMAKE_INSTALL_PREFIX})