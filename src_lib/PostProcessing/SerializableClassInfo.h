#pragma once

#include <string>
#include <vector>
#include "SerializationErrors.h"
#include "SerializeFunctionInfo.h"
#include <memory>

#include "clang/AST/DeclTemplate.h"

class SerializableClassInfo;
class SerializableFieldInfo;

using SerializableClassName = std::string;
using SerializableClassInfoPtr = std::shared_ptr<SerializableClassInfo>;
using SerializableClassInfoWeakPtr = std::weak_ptr<const SerializableClassInfo>;

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

    void GenerateSerializeMethodInfo();

    void RunSerializeMethodAnalysis();
};