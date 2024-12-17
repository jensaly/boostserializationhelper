#include "SerializableClassInfo.h"

std::vector<SerializableFieldInfo> const& SerializableClassInfo::GetFields() const {
    return m_fields;
}

SerializableClassInfo::SerializableClassInfo(std::string className) : m_className{className} {

}

SerializableClassName SerializableClassInfo::GetClassName() const {
    return m_className;
}

void SerializableClassInfo::AddSerializableField(SerializableFieldInfo fieldInfo) {
    m_fields.push_back(fieldInfo);
}

void SerializableClassInfo::SetError(SerializationError error) {
    m_errors |= error;
}

bool SerializableClassInfo::HasError(SerializationError error) const {
    if (error != SerializationError::Error_NoError) 
        return (m_errors & error) != SerializationError::Error_NoError;
    return true;
}