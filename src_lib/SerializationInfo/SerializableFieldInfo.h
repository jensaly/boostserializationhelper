#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializationObject.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================
class SerializeOperationInfo;

class SerializableFieldInfo : public SerializationObject {
public:
    SerializableFieldInfo(std::string name, std::string filename, unsigned int line, unsigned int column);

    bool operator==(const SerializeOperationInfo& field);
};
