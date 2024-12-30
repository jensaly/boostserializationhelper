#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <vector>

// ==================================
// Internal Headers
// ==================================
#include <Types/InfoTypes.h>
#include <SerializationInfo/SerializationErrors.h>
#include <SerializationInfo/SerializeFunctionInfo.h>

// ==================================
// Libtooling Headers
// ==================================
#include "clang/AST/DeclTemplate.h"

// ==================================
// Forward Declarations
// ==================================


class SerializableClassInfo : public SerializationObject {
    SerializationErrorFlag m_errorFlags = SerializationErrorFlag::Error_NoError; // Just the error flags set for the object.
    SerializationInfoFlags m_info = SerializationInfoFlags::Info_NoInfo;

    std::vector<std::shared_ptr<SerializableFieldInfo>> m_fields; // Serializable fields inside of the class

    std::shared_ptr<SerializeFunctionInfo> m_methodInfo = nullptr; // Pointer to its serialize-function information

    std::vector<std::unique_ptr<SerializationError>> m_errors;

public:
    SerializationErrorFlag GetErrors() const { return m_errorFlags; }
    std::vector<std::shared_ptr<SerializableFieldInfo>> const& GetFields() const;

    bool SetSerializeMethodInfo(std::shared_ptr<SerializeFunctionInfo> serializeFunctionInfo);

    SerializableClassInfo(std::string className, std::string filename, unsigned int line, unsigned int column);

    SerializableClassName GetClassName() const;

    void AddSerializableField(std::shared_ptr<SerializableFieldInfo>&& fieldInfo);

    void SetError(SerializationErrorFlag error);

    void SetError(std::unique_ptr<SerializationError>&& error);

    bool HasError(SerializationErrorFlag error) const;

    bool HasSerializeMethod() const;

    void RunSerializeMethodAnalysis();
};