set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS 
        Internals/Internals.h
        Internals/SerializationContext.h
        SerializableClassInfoMediator.h
        PostProcessing/SerializationErrors.h
        PostProcessing/SerializableClassInfo.h
        PostProcessing/SerializableFieldInfo.h
        PostProcessing/SerializeFunctionInfo.h
        PostProcessing/SerializeOperationInfo.h
    )

set(SOURCES
        Internals/Internals.cpp
        Internals/SerializationContext.cpp
        SerializableClassInfoMediator.cpp
        PostProcessing/SerializableClassInfo.cpp
        PostProcessing/SerializableFieldInfo.cpp
        PostProcessing/SerializeFunctionInfo.cpp
        PostProcessing/SerializeOperationInfo.cpp
    )

add_library(sh_lib STATIC ${HEADERS} ${SOURCES})

target_link_libraries(sh_lib PUBLIC common clangTooling clangBasic clangASTMatchers)

target_include_directories(sh_lib SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/common)

target_compile_options(sh_lib PRIVATE -fno-limit-debug-info)