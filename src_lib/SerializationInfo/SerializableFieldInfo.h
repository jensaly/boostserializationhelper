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
class SerializeOperationInfo;

class SerializableFieldInfo {
    std::string m_name;
public:
    SerializableFieldInfo(std::string name) : m_name{name} {}

    std::string GetName() const { return m_name; }

    bool operator==(const SerializeOperationInfo& field);
};
