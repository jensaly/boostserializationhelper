add_executable(SerializationHelper main.cpp)

target_link_libraries(SerializationHelper PUBLIC sh_lib)

target_include_directories(SerializationHelper SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src_lib)