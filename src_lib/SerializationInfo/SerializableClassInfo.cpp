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

bool SerializableClassInfo::SetSerializeMethodInfo(std::shared_ptr<ISerializeFunctionInfo> serializeFunctionInfo) {
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

void SerializableClassInfo::SetInfo(SerializationInfoFlag info) {
    m_infoFlags |= info;
}

void SerializableClassInfo::UnsetInfo(SerializationInfoFlag info) {
    m_infoFlags &= ~info;
}

bool SerializableClassInfo::HasInfo(SerializationInfoFlag info) const {
    if (info != SerializationInfoFlag::Info_NoInfo) {
        return (m_infoFlags & info) != SerializationInfoFlag::Info_NoInfo;
    }
    else {
        return m_infoFlags == info;
    }
}

size_t SerializableClassInfo::NumberOfErrors() const {
    return m_errors.size();
}

bool SerializableClassInfo::HasSerializeMethod() const {
    return m_methodInfo != nullptr;
}

void SerializableClassInfo::RunSerializeMethodAnalysis() {
    SerializationErrorFlag error = SerializationErrorFlag::Error_NoError;

    if (m_methodInfo == nullptr) {
        // May previously have assumed non-intrusive. However, if the class still has no serialize-method, it does not exist.
        // Unset the non-intrusive.
        if (HasInfo(SerializationInfoFlag::Info_NonIntrusive)) {
            UnsetInfo(SerializationInfoFlag::Info_NonIntrusive);
        }
        auto error = std::make_unique<SerializationError_SerializeMethodNotFound>(shared_from_this());
        SetError(std::move(error));
        return;
    }

    m_methodInfo->RunChecks(shared_from_this());
}

void SerializableClassInfo::Log(std::vector<std::string>& output) {
    for (auto& error : m_errors) {
        error->ToString(output);
    }
}