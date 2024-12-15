#pragma once

#include <string>
#include <vector>
#include "SerializationErrors.h"
#include <memory>

#include "clang/AST/DeclTemplate.h"

class SerializableClassInfo;

using SerializableClassName = std::string;
using SerializableClassInfoPtr = std::shared_ptr<SerializableClassInfo>;
using SerializableClassInfoWeakPtr = std::weak_ptr<const SerializableClassInfo>;

class SerializableFieldInfo {
    std::string m_name;
public:
    SerializableFieldInfo(std::string name) : m_name{name} {}

    std::string GetName() const { return m_name; }
};

// Interface for handling of inline, non-intrusive and split serialize methods
class ISerializeMethodInfo {
public:
    virtual void RunChecks(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo) = 0;
    virtual ~ISerializeMethodInfo() {}
};

class SerializeMethodInline : public ISerializeMethodInfo {
public:
    void RunChecks(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo) override;
    SerializeMethodInline() = default;
    ~SerializeMethodInline() override = default;
};

class SerializeMethodNonIntrusive {

};

class SerializeMethodSplitInline {

};

class SerializeMethodSplitNonIntrusive {

};

class SerializableClassInfo {
    std::string m_className;
    SerializationError m_errors = SerializationError::Error_NoError;
    SerializationInformation m_info = SerializationInformation::Info_NoInfo;

    std::vector<SerializableFieldInfo> m_fields;

    std::unique_ptr<ISerializeMethodInfo> m_methodInfo;

public:
    SerializationError GetErrors() const { return m_errors; }
    std::vector<SerializableFieldInfo> const& GetFields() const;

    SerializableClassInfo(std::string className);

    SerializableClassName GetClassName() const;

    void AddSerializableField(SerializableFieldInfo fieldInfo);

    void SetError(SerializationError error);

    bool HasError(SerializationError error) const;

    void GenerateSerializeMethodInfo();

    void RunSerializeMethodAnalysis();
};