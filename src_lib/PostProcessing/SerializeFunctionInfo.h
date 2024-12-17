#pragma once

#include <string>
#include <vector>
#include "SerializationErrors.h"
#include <memory>

#include "clang/AST/DeclTemplate.h"

// Interface for handling of inline, non-intrusive and split serialize methods
class ISerializeFunctionInfo {
public:
    virtual void RunChecks(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo) = 0;
    virtual ~ISerializeFunctionInfo() {}
};

class SerializeFunctionInfo {
    SerializationError m_errors = SerializationError::Error_NoError;
    SerializationInformation m_info = SerializationInformation::Info_NoInfo;

    clang::FunctionTemplateDecl* m_decl = nullptr;
    //CXXRecordDecl* classDecl = nullptr; // Used for reverse lookup in processing.

public:
    SerializationError GetErrors() const { return m_errors; }
    std::vector<SerializableFieldInfo> const& GetFields() const;

    SerializeFunctionInfo(clang::FunctionTemplateDecl* decl);

    virtual ~SerializeFunctionInfo() { }

    void AddSerializableField(SerializableFieldInfo fieldInfo);

    void SetError(SerializationError error);

    bool HasError(SerializationError error) const;

    void GenerateSerializeMethodInfo();

    void RunSerializeMethodAnalysis();
};

// Created by CXXRecordDecl during discovery, immediately associated
class SerializeFunctionInfo_Intrusive : SerializeFunctionInfo {
    SerializeFunctionInfo_Intrusive() = delete;
    SerializeFunctionInfo_Intrusive(clang::FunctionTemplateDecl* decl);
    ~SerializeFunctionInfo_Intrusive() override = default;
};

// Created freely during discovery, associated later.
class SerializeFunctionInfo_NonIntrusive : SerializeFunctionInfo {
    SerializeFunctionInfo_NonIntrusive() = delete;
    SerializeFunctionInfo_NonIntrusive(clang::FunctionTemplateDecl* decl);
    ~SerializeFunctionInfo_NonIntrusive() override = default;
};