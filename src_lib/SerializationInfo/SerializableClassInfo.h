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


class SerializableClassInfo {
    std::string m_className;
    SerializationError m_errors = SerializationError::Error_NoError;
    SerializationInformation m_info = SerializationInformation::Info_NoInfo;

    std::vector<SerializableFieldInfo> m_fields; // Serializable fields inside of the class

    std::shared_ptr<SerializeFunctionInfo> m_methodInfo = nullptr; // Pointer to its serialize-function information

public:
    SerializationError GetErrors() const { return m_errors; }
    std::vector<SerializableFieldInfo> const& GetFields() const;

    bool SetSerializeMethodInfo(std::shared_ptr<SerializeFunctionInfo> serializeFunctionInfo);

    SerializableClassInfo(std::string className);

    SerializableClassName GetClassName() const;

    void AddSerializableField(SerializableFieldInfo fieldInfo);

    void SetError(SerializationError error);

    bool HasError(SerializationError error) const;

    bool HasSerializeMethod() const;

    void RunSerializeMethodAnalysis();
};