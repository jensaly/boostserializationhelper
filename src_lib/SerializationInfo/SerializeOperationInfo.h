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
    std::string m_operationName;
    std::string m_operationFilename;
    unsigned int m_operationLine;
    unsigned int m_operationColumn;
public:
    SerializeOperationInfo(std::string operationName, std::string operationFilename, unsigned int operationLine, unsigned int operationColumn,
        std::string declName, std::string declFilename, unsigned int declLine, unsigned int declColumn
    );

    std::string GetOperationName() const;

    std::string GetOperationFilename() const;
    
    unsigned int GetOperationLine() const;

    unsigned int GetOperationColumn() const;
};