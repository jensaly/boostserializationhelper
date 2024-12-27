#pragma once

#include <string>
#include <vector>
#include "SerializationErrors.h"
#include <memory>

#include "clang/AST/DeclTemplate.h"

class SerializableClassInfo;
using SerializableClassInfoPtr = std::shared_ptr<SerializableClassInfo>;
class SerializeOperationInfo;

// Interface for handling of inline, non-intrusive and split serialize methods
class ISerializeFunctionInfo {
public:
    virtual void RunChecks(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo) = 0;
    virtual ~ISerializeFunctionInfo() {}
};

class SerializeFunctionInfo {
    SerializationError m_errors = SerializationError::Error_NoError;
    SerializationInformation m_info = SerializationInformation::Info_NoInfo;

    clang::FunctionDecl* m_decl = nullptr;
    
    std::vector<SerializeOperationInfo> m_operationsInfo;

public:
    SerializationError GetErrors() const { return m_errors; }
    std::vector<SerializeOperationInfo> const& GetFields() const { return m_operationsInfo; };

    SerializeFunctionInfo(clang::FunctionDecl* decl);

    virtual ~SerializeFunctionInfo();

    void AddSerializableField(SerializeOperationInfo operationInfo);

    void SetError(SerializationError error);

    bool HasError(SerializationError error) const;

    void GenerateSerializeMethodInfo();

    void RunSerializeMethodAnalysis();
};

// Created by CXXRecordDecl during discovery, immediately associated
class SerializeFunctionInfo_Intrusive : public SerializeFunctionInfo {
public:
    SerializeFunctionInfo_Intrusive() = delete;
    SerializeFunctionInfo_Intrusive(clang::FunctionDecl* decl);
    ~SerializeFunctionInfo_Intrusive() override = default;
};

// Created freely during discovery, associated later.
class SerializeFunctionInfo_NonIntrusive : public SerializeFunctionInfo {
public:
    SerializeFunctionInfo_NonIntrusive() = delete;
    SerializeFunctionInfo_NonIntrusive(clang::FunctionDecl* decl);
    ~SerializeFunctionInfo_NonIntrusive() override = default;
};