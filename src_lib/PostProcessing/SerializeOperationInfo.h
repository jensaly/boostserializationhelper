#pragma once

#include <string>

class SerializableFieldInfo;

class SerializeOperationInfo {
    std::string m_name;
public:
    SerializeOperationInfo(std::string name) : m_name{name} {}

    std::string GetName() const { return m_name; }

    
};