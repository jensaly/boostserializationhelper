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
#include <SerializationInfo/SerializationObject.h>

// ==================================
// Libtooling Headers
// ==================================
#include "clang/AST/DeclTemplate.h"

// ==================================
// Forward Declarations
// ==================================

// Interface for handling of inline, non-intrusive and split serialize methods
class ISerializeFunctionInfo {
public:
    virtual void RunChecks(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo) = 0;
    virtual ~ISerializeFunctionInfo() {}
};

class SerializeFunctionInfo : public SerializationObject {
    SerializationErrorFlag m_errorFlags = SerializationErrorFlag::Error_NoError;
    SerializationInfoFlags m_info = SerializationInfoFlags::Info_NoInfo;

    clang::FunctionDecl* m_decl = nullptr;
    
    std::vector<SerializeOperationInfoPtr> m_operationsInfo;

public:
    SerializationErrorFlag GetErrors() const { return m_errorFlags; }
    std::vector<SerializeOperationInfoPtr> const& GetFields() const { return m_operationsInfo; };

    SerializeFunctionInfo(std::string funcName, std::string filename, unsigned int line, unsigned int column);

    virtual ~SerializeFunctionInfo();

    void AddSerializableField(SerializeOperationInfoPtr&& operationInfo);

    void SetError(SerializationErrorFlag error);

    bool HasError(SerializationErrorFlag error) const;

    void GenerateSerializeMethodInfo();

    void RunSerializeMethodAnalysis();
};

// Created by CXXRecordDecl during discovery, immediately associated
class SerializeFunctionInfo_Intrusive : public SerializeFunctionInfo {
public:
    SerializeFunctionInfo_Intrusive() = delete;
    SerializeFunctionInfo_Intrusive(std::string funcName, std::string filename, unsigned int line, unsigned int column);
    ~SerializeFunctionInfo_Intrusive() override = default;
};

// Created freely during discovery, associated later.
class SerializeFunctionInfo_NonIntrusive : public SerializeFunctionInfo {
public:
    SerializeFunctionInfo_NonIntrusive() = delete;
    SerializeFunctionInfo_NonIntrusive(std::string funcName, std::string filename, unsigned int line, unsigned int column);
    ~SerializeFunctionInfo_NonIntrusive() override = default;
};