cmake_minimum_required(VERSION 3.20)

project(SerializationHelper)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${Clang_INCLUDE_DIRS})
include_directories(${LLVM_LIBRARY_DIRS} ${Clang_LIBRARY_DIRS})

add_executable(SerializationHelper src/main.cpp src/basic.cpp)

target_link_libraries(SerializationHelper clangTooling clangBasic clangASTMatchers)