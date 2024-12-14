#include "SerializableClassInfo.h"

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
    return (m_errors & error) != SerializationError::Error_NoError;
}
