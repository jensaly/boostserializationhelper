set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS visitor.h)

set(SOURCES)

add_library(sh_lib STATIC ${HEADERS} ${SOURCES})

target_link_libraries(sh_lib common clangTooling clangBasic clangASTMatchers)

target_include_directories(sh_lib PUBLIC ${CMAKE_SOURCE_DIR}/common)