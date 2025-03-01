#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

class SerializableFieldInfo;

class SerializeOperationInfo {
    std::string m_name;
public:
    SerializeOperationInfo(std::string name) : m_name{name} {}

    std::string GetName() const { return m_name; }

    
};