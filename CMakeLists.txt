cmake_minimum_required(VERSION 3.20)

project(SerializationHelper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Compiler:                    ${CMAKE_CXX_COMPILER_ID}" )
message(STATUS "Build type:                  ${CMAKE_BUILD_TYPE}")
message(STATUS "Binary dir:                  ${CMAKE_BINARY_DIR}")
message(STATUS "Install dir:                 ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Runtime Output:              ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Build component:             ${BUILD_COMPONENT}")

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${Clang_INCLUDE_DIRS})
include_directories(${LLVM_LIBRARY_DIRS} ${Clang_LIBRARY_DIRS})

add_subdirectory(common)

if (BUILD_COMPONENT STREQUAL "deploy")
    message(STATUS "Building Normal")
    add_subdirectory(src)
    add_subdirectory(src_lib)
elseif(BUILD_COMPONENT STREQUAL "test_projects")
    message(STATUS "Building project tests")
    add_subdirectory(projects)
    add_subdirectory(test_projects)
elseif(BUILD_COMPONENT STREQUAL "test_tool")
    message(STATUS "Building tool tests")
    add_subdirectory(projects)
    add_subdirectory(src_lib)
    add_subdirectory(test_main)
else()
    message(ERROR "Unknown component")
endif()