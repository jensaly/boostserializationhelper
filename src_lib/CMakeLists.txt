set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS)

set(SOURCES basic.cpp)

add_library(sh_lib STATIC ${HEADERS} ${SOURCES})

target_link_libraries(sh_lib clangTooling clangBasic clangASTMatchers)