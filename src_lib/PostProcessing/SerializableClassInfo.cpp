#include "SerializableClassInfo.h"
#include "SerializableFieldInfo.h"
#include "SerializeOperationInfo.h"

std::vector<SerializableFieldInfo> const& SerializableClassInfo::GetFields() const {
    return m_fields;
}

bool SerializableClassInfo::SetSerializeMethodInfo(std::shared_ptr<SerializeFunctionInfo> serializeFunctionInfo) {
    if (m_methodInfo != nullptr) {
        return false;
    }
    m_methodInfo = serializeFunctionInfo;
    return true;
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

bool SerializableClassInfo::HasSerializeMethod() const {
    return m_methodInfo != nullptr;
}

void SerializableClassInfo::RunSerializeMethodAnalysis() {
    SerializationError error = SerializationError::Error_NoError;

    if (m_methodInfo == nullptr) {
        SetError(SerializationError::Error_SerializeMethodNotFound);
        return;
    }

    auto methodContents = m_methodInfo->GetFields();
    auto classFields = GetFields();

    for (auto& field : classFields) {
        if (std::find_if(methodContents.begin(), methodContents.end(), [&](SerializeOperationInfo& operationInfo){ 
            return field == operationInfo;
        }) == methodContents.end()) {
            SetError(SerializationError::Error_MarkedFieldNotSerialized);
        }
    }

    for (auto& field : methodContents) {
        if (std::find_if(classFields.begin(), classFields.end(), [&](SerializableFieldInfo& operationInfo){ 
            return field == operationInfo;
        }) == classFields.end()) {
            SetError(SerializationError::Error_UnmarkedFieldSerialized);
        }
    }
}