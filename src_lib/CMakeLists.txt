set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS serializable_class.h serializable_class_storage.h)

set(SOURCES serializable_class.cpp serializable_class_storage.cpp)

add_library(sh_lib STATIC ${HEADERS} ${SOURCES})

target_link_libraries(sh_lib PUBLIC common clangTooling clangBasic clangASTMatchers)

target_include_directories(sh_lib SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/common)

target_compile_options(sh_lib PRIVATE -fno-limit-debug-info)