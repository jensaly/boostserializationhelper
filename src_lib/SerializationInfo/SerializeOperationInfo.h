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

class SerializableFieldInfo;

// Class for storing a single serialization operator (ar & field)

class SerializeOperationInfo : public SerializationObject{
public:
    SerializeOperationInfo(std::string operationName, std::string filename, unsigned int line, unsigned int column);
};