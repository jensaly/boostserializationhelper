set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e3ddede6c4ee818825c4e5a6dfa1d384860c27d9) # 10.2.1
FetchContent_MakeAvailable(fmt)

set(HEADERS
        Diagnostics/DiagnosticMessage.h
        Diagnostics/IDiagnosticReporter.h
        Diagnostics/TestDiagnosticReporter.h
        Internals/Internals.h
        SerializationContext.h
        Mediation/SerializableClassInfoMediator.h
        SerializationInfo/InfoFactory.h
        SerializationInfo/SerializationErrors.h
        SerializationInfo/SerializationObject.h
        SerializationInfo/SerializableClassInfo.h
        SerializationInfo/SerializableFieldInfo.h
        SerializationInfo/SerializeFunctionInfo.h
        SerializationInfo/SerializeOperationInfo.h
        Types/InfoTypes.h
        Utils/Utils.h
    )

set(SOURCES
        Diagnostics/DiagnosticMessage.cpp
        Diagnostics/TestDiagnosticReporter.cpp
        Internals/Internals.cpp
        SerializationContext.cpp
        Mediation/SerializableClassInfoMediator.cpp
        SerializationInfo/SerializationErrors.cpp
        SerializationInfo/SerializationObject.cpp
        SerializationInfo/SerializableClassInfo.cpp
        SerializationInfo/SerializableFieldInfo.cpp
        SerializationInfo/SerializeFunctionInfo.cpp
        SerializationInfo/SerializeOperationInfo.cpp
        Utils/Utils.cpp
    )

add_library(sh_lib STATIC ${HEADERS} ${SOURCES})

target_link_libraries(sh_lib PUBLIC common clangTooling clangBasic clangASTMatchers fmt::fmt)

target_include_directories(sh_lib SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/common)

target_compile_options(sh_lib PRIVATE -fno-limit-debug-info)