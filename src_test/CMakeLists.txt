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

include_directories(${GTEST_INCLUDE_DIRS})

set(SLAVE_TEST_SOURCES main.cpp)

set(SLAVE_TEST_HEADERS)

include(GoogleTest)

find_package(Qt6 COMPONENTS Test REQUIRED)

add_executable(sh_test ${SLAVE_TEST_SOURCES} ${SLAVE_TEST_HEADERS})
target_link_libraries(sh_test gtest gtest_main)

gtest_discover_tests(sh_test)

install(TARGETS sh_test
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)