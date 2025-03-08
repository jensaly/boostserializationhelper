#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <vector>
#include <memory>

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


class SerializableClassInfo : public SerializationObject, public std::enable_shared_from_this<SerializableClassInfo> {
    SerializationErrorFlag m_errorFlags = SerializationErrorFlag::Error_NoError; // Just the error flags set for the object. A summrary.
    SerializationInfoFlag m_infoFlags = SerializationInfoFlag::Info_NoInfo;

    std::vector<std::shared_ptr<SerializableFieldInfo>> m_fields; // Serializable fields inside of the class

    std::shared_ptr<ISerializeFunctionInfo> m_methodInfo = nullptr; // Pointer to its serialize-function information

    std::vector<std::unique_ptr<SerializationError>> m_errors;

    //std::vector<std::unique_ptr<SerializationInfo>> m_info;

public:
    SerializationErrorFlag GetErrors() const { return m_errorFlags; }
    std::vector<std::shared_ptr<SerializableFieldInfo>> const& GetFields() const;

    bool SetSerializeMethodInfo(std::shared_ptr<ISerializeFunctionInfo> serializeFunctionInfo);

    SerializableClassInfo(std::string className, std::string filename, unsigned int line, unsigned int column);

    SerializableClassName GetClassName() const; // TODO: Needs to save whether its a class or a struct as well

    void AddSerializableField(std::shared_ptr<SerializableFieldInfo>&& fieldInfo);

    void SetError(SerializationErrorFlag error);

    void SetError(std::unique_ptr<SerializationError>&& error);

    bool HasError(SerializationErrorFlag error) const;

    void SetInfo(SerializationInfoFlag info);

    void SetInfo(std::unique_ptr<SerializationInfoFlag>&& info);

    bool HasInfo(SerializationInfoFlag error) const;

    size_t NumberOfErrors() const;

    bool HasSerializeMethod() const;

    void RunSerializeMethodAnalysis();

    void Log(std::vector<std::string>& output);
};