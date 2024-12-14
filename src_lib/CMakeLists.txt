set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS 
        Internals/Internals.h
        SerializableClassInfoAggregator.h
        PostProcessing/SerializationErrors.h
        PostProcessing/SerializableClassInfo.h
    )

set(SOURCES
        Internals/Internals.cpp
        SerializableClassInfoAggregator.cpp
        PostProcessing/SerializableClassInfo.cpp
    )

add_library(sh_lib STATIC ${HEADERS} ${SOURCES})

target_link_libraries(sh_lib PUBLIC common clangTooling clangBasic clangASTMatchers)

target_include_directories(sh_lib SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/common)

target_compile_options(sh_lib PRIVATE -fno-limit-debug-info)