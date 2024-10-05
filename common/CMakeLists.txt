set(HEADERS serializable.h)

add_library(common STATIC ${HEADERS} ${SOURCES})

set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)