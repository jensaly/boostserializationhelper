#include <string>
#include <vector>
#include "SerializationErrors.h"

using SerializableClassName = std::string;

class SerializableFieldInfo {
    std::string m_name;
public:
    SerializableFieldInfo(std::string name) : m_name{name} {}
};

class SerializableClassInfo {
    std::string m_className;
    SerializationError m_errors = SerializationError::Error_NoError;
    SerializationInformation m_info = SerializationInformation::Info_NoInfo;

    std::vector<SerializableFieldInfo> m_fields;

public:
    SerializableClassInfo(std::string className);

    SerializableClassName GetClassName() const;

    void AddSerializableField(SerializableFieldInfo fieldInfo);

    void SetError(SerializationError error);

    SerializationError GetErrors() const { return m_errors; }

    bool HasError(SerializationError error) const;
};