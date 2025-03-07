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
    virtual void RunChecks(SerializableClassInfoPtr classInfo) = 0;
    virtual ~ISerializeFunctionInfo() {}
};

class SerializeFunctionInfo : public ISerializeFunctionInfo, public SerializationObject {
    SerializationErrorFlag m_errorFlags = SerializationErrorFlag::Error_NoError;
    SerializationInfoFlag m_info = SerializationInfoFlag::Info_NoInfo;

    clang::FunctionDecl* m_decl = nullptr;
    
    std::vector<SerializeOperationInfoPtr> m_operationsInfo;

    bool m_splitsInternally;

public:
    SerializationErrorFlag GetErrors() const { return m_errorFlags; }
    std::vector<SerializeOperationInfoPtr> const& GetFields() const { return m_operationsInfo; };

    SerializeFunctionInfo(std::string funcName, std::string filename, unsigned int line, unsigned int column);

    virtual ~SerializeFunctionInfo();

    void RunChecks(SerializableClassInfoPtr classInfo) override;

    void AddSerializableField(SerializeOperationInfoPtr&& operationInfo);

    void SetError(SerializationErrorFlag error);
    bool HasError(SerializationErrorFlag error) const;

    void SetSplitsInternally(bool splitsInternally);
    bool SplitsInternally() const;

    friend class SplitFunctionInfo_Intrusive;
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

// Wrapper that implements the same interface as the single serialize-function, but actually stores two as SerializationObjects
class SplitFunctionInfo_Intrusive : public ISerializeFunctionInfo {
    SerializeFunctionInfoPtr m_save = nullptr;
    SerializeFunctionInfoPtr m_load = nullptr;

public:
    SplitFunctionInfo_Intrusive(SerializeFunctionInfoPtr&& save, SerializeFunctionInfoPtr&& load);

    virtual ~SplitFunctionInfo_Intrusive();

    void RunChecks(SerializableClassInfoPtr classInfo) override;
};