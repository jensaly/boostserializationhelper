set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
        Diagnostics/DiagnosticMessage.h
        Diagnostics/IDiagnosticReporter.h
        Diagnostics/TestDiagnosticReporter.h
        Internals/Internals.h
        SerializationContext.h
        Mediation/SerializableClassInfoMediator.h
        SerializationInfo/SerializationErrors.h
        SerializationInfo/SerializableClassInfo.h
        SerializationInfo/SerializableFieldInfo.h
        SerializationInfo/SerializeFunctionInfo.h
        SerializationInfo/SerializeOperationInfo.h
        Types/InfoTypes.h
        Utils/Utils.h
    )

set(SOURCES
        Diagnostics/DiagnosticMessage.cpp
        Internals/Internals.cpp
        SerializationContext.cpp
        Mediation/SerializableClassInfoMediator.cpp
        SerializationInfo/SerializationErrors.cpp
        SerializationInfo/SerializableClassInfo.cpp
        SerializationInfo/SerializableFieldInfo.cpp
        SerializationInfo/SerializeFunctionInfo.cpp
        SerializationInfo/SerializeOperationInfo.cpp
        Utils/Utils.cpp
    )

add_library(sh_lib STATIC ${HEADERS} ${SOURCES})

target_link_libraries(sh_lib PUBLIC common clangTooling clangBasic clangASTMatchers)

target_include_directories(sh_lib SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/common)

target_compile_options(sh_lib PRIVATE -fno-limit-debug-info)