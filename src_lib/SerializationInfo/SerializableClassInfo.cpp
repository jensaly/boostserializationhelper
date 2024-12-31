// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializableClassInfo.h>
#include <SerializationInfo/SerializableFieldInfo.h>
#include <SerializationInfo/SerializeOperationInfo.h>
#include <Types/InfoTypes.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

std::vector<std::shared_ptr<SerializableFieldInfo>> const& SerializableClassInfo::GetFields() const {
    return m_fields;
}

bool SerializableClassInfo::SetSerializeMethodInfo(std::shared_ptr<SerializeFunctionInfo> serializeFunctionInfo) {
    if (m_methodInfo != nullptr) {
        return false;
    }
    m_methodInfo = serializeFunctionInfo;
    return true;
}

SerializableClassInfo::SerializableClassInfo(std::string className, std::string filename, unsigned int line, unsigned int column)
        : SerializationObject(className, filename, line, column)
{

}

SerializableClassName SerializableClassInfo::GetClassName() const {
    return GetName();
}

void SerializableClassInfo::AddSerializableField(std::shared_ptr<SerializableFieldInfo>&& fieldInfo) {
    m_fields.push_back(std::move(fieldInfo));
}

void SerializableClassInfo::SetError(SerializationErrorFlag error) {
    m_errorFlags |= error;
}

void SerializableClassInfo::SetError(std::unique_ptr<SerializationError>&& error) {
    SetError(error->m_flag);
    m_errors.push_back(std::move(error));
}

bool SerializableClassInfo::HasError(SerializationErrorFlag error) const {
    if (error != SerializationErrorFlag::Error_NoError) {
        return (m_errorFlags & error) != SerializationErrorFlag::Error_NoError;
    }
    else {
        return m_errorFlags == error;
    }
}

bool SerializableClassInfo::HasSerializeMethod() const {
    return m_methodInfo != nullptr;
}

void SerializableClassInfo::RunSerializeMethodAnalysis() {
    SerializationErrorFlag error = SerializationErrorFlag::Error_NoError;

    if (m_methodInfo == nullptr) {
        auto error = std::make_unique<SerializationError_SerializeMethodNotFound>(shared_from_this());
        SetError(std::move(error));
        return;
    }

    auto methodContents = m_methodInfo->GetFields();
    auto classFields = GetFields();

    for (auto& field : classFields) {
        auto operation_it = std::find_if(methodContents.begin(), methodContents.end(), [&](SerializeOperationInfoPtr& operationInfo){ 
            return field->GetName() == operationInfo->GetName();
        });
        if (operation_it == methodContents.end()) {
            auto& operation = *operation_it;
            // No serialize operations could be matched to the content of the class definition
            std::unique_ptr<SerializationError> error = std::make_unique<SerializationError_MarkedFieldNotSerialized>(
                *field, *m_methodInfo, shared_from_this()
            );
            SetError(std::move(error));
        }
    }

    for (auto& operationInfo : methodContents) {
        auto field_it = std::find_if(classFields.begin(), classFields.end(), [&](SerializableFieldInfoPtr& fieldInfo){ 
            return operationInfo->GetName() == fieldInfo->GetName();
        });
        if (field_it == classFields.end()) {
            std::unique_ptr<SerializationError> error = std::make_unique<SerializationError_UnmarkedFieldSerialized>(
                *operationInfo, *m_methodInfo, shared_from_this()
            );
            SetError(std::move(error));
        }
    }
}

void SerializableClassInfo::Log(std::vector<std::string>& output) {
    for (auto& error : m_errors) {
        error->ToString(output);
    }
}