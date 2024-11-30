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
target_link_libraries(test_main basic sh_lib gtest gtest_main sh_lib common)

target_include_directories(test_main PUBLIC ${CMAKE_SOURCE_DIR}/projects ${CMAKE_SOURCE_DIR}/src_lib ${CMAKE_SOURCE_DIR}/common)

target_compile_options(test_main PRIVATE -fno-limit-debug-info)

gtest_discover_tests(test_main)

# Recursively get all .cpp and .hpp files
file(GLOB_RECURSE source_files
    ${CMAKE_SOURCE_DIR}/projects/*.cpp
    ${CMAKE_SOURCE_DIR}/projects/*.hpp
)

# Install each file, preserving the folder structure
foreach(file ${source_files})
    # Get the relative path by stripping the source directory prefix
    file(RELATIVE_PATH rel_path ${CMAKE_SOURCE_DIR}/projects ${file})

    # Extract the directory part of the relative path
    get_filename_component(dir ${rel_path} DIRECTORY)

    # Install the file, ensuring the correct directory structure is preserved
    install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/projects/${dir})
endforeach()

install(TARGETS test_main DESTINATION ${CMAKE_INSTALL_PREFIX})