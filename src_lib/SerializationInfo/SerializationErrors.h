#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <cstdint>
#include <vector>
#include <string>

// ==================================
// Internal Headers
// ==================================
#include <Types/InfoTypes.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

class SerializeOperationInfo;
class SerializableFieldInfo;
class SerializeFunctionInfo;

enum class SerializationErrorFlag : uint64_t {
    Error_NoError = 0,
    Error_MarkedFieldNotSerialized = 1 << 0, // A field marked for serialization is not present in the serialize/save/load method
    Error_UnmarkedFieldSerialized = 1 << 1, // A field not marked for serialization is present in the serialize method
    Error_SerializeMethodNotFound = 1 << 2, // No serialize-method is found (TODO: Reconsider, the compile may handle this)
    Error_SaveLoadOrderMismatched = 1 << 3 // In the split save-load methods, the order of operations is not the same
};

std::string toStringSep(SerializationErrorFlag flag);

enum class SerializationInfoFlags : uint64_t {
    Info_NoInfo = 0,
    Info_NonIntrusive = 1 << 0, // The class's serialization functionality is non-intrusive.
    Info_SplitSerialization = 1 << 1, // The class uses a split save/load
};

inline SerializationErrorFlag operator|(SerializationErrorFlag lhs, SerializationErrorFlag rhs) {
    return static_cast<SerializationErrorFlag>(
        static_cast<uint64_t>(lhs) | static_cast<uint64_t>(rhs)
    );
}

inline SerializationErrorFlag& operator|=(SerializationErrorFlag& lhs, SerializationErrorFlag rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline SerializationErrorFlag operator&(SerializationErrorFlag lhs, SerializationErrorFlag rhs) {
    return static_cast<SerializationErrorFlag>(
        static_cast<uint64_t>(lhs) & static_cast<uint64_t>(rhs)
    );
}

class SerializationError {
protected:
    SerializableClassInfoWeakPtr m_class;
public:
    SerializationErrorFlag m_flag = SerializationErrorFlag::Error_NoError;
    SerializationError() = default;

    // Used to create errors noe tied to a specific location
    SerializationError(SerializationErrorFlag flag, SerializableClassInfoPtr& owningClass)
            : m_flag{flag}, m_class{owningClass} {}

    virtual ~SerializationError() {}
    
    SerializationErrorFlag GetFlag() const { return m_flag; }

    virtual void ToString(std::vector<std::string>& output) const = 0;
};

class SerializationError_MarkedFieldNotSerialized : public SerializationError {
private:
    std::string m_fieldName = ";;UNKNOWN;;";
    std::string m_filenameDecl = ";;UNKNOWN;;";
    unsigned int m_declLine; // Max value indicates that this error isn't related to a specific line
    unsigned int m_declColumn; // Max value indicates that this error isn't related to a specific column
    std::string m_filenameSave = ";;UNKNOWN;;";
    unsigned int m_saveLine;
    unsigned int m_saveColumn;
    bool m_split = false; // If set to false, there's a single serialize-function. 

public:
    // For split-member serialization
    //SerializationError_MarkedFieldNotSerialized(SerializeFie);
    
    // For normal serialization
    SerializationError_MarkedFieldNotSerialized(SerializableFieldInfo& field, SerializeFunctionInfo& method, SerializableClassInfoPtr owningClass);

    void ToString(std::vector<std::string>& output) const override;

    ~SerializationError_MarkedFieldNotSerialized() override = default;
};

class SerializationError_UnmarkedFieldSerialized : public SerializationError {
private:
    std::string m_fieldName = ";;UNKNOWN;;";
    std::string m_filenameDecl = ";;UNKNOWN;;";
    unsigned int m_declLine;
    unsigned int m_declColumn;
    std::string m_filenameSave = ";;UNKNOWN;;";
    unsigned int m_saveLine;
    unsigned int m_saveColumn;
    std::string m_filenameLoad = ";;UNKNOWN;;";
    unsigned int m_loadLine;
    unsigned int m_loadColumn;

public:
    SerializationError_UnmarkedFieldSerialized(SerializeOperationInfo& operation, SerializeFunctionInfo& function, SerializableClassInfoPtr owningClass);

    void ToString(std::vector<std::string>& output) const override;

    ~SerializationError_UnmarkedFieldSerialized() override = default;
};

class SerializationError_SaveLoadOrderMismatched : public SerializationError {
private:
    std::string m_fieldName = ";;UNKNOWN;;";
    std::string m_filenameSave = ";;UNKNOWN;;";
    unsigned int m_saveLine;
    unsigned int m_saveColumn;
    std::string m_filenameLoad = ";;UNKNOWN;;";
    unsigned int m_loadLine;
    unsigned int m_loadColumn; 

public:
    // For split-member serialization
    SerializationError_SaveLoadOrderMismatched(SerializeOperationInfo& loadOperation, SerializeOperationInfo& saveOperation, SerializeFunctionInfo& saveFunction, SerializeFunctionInfo& loadFunction, SerializableClassInfoPtr owningClass);

    void ToString(std::vector<std::string>& output) const override;

    ~SerializationError_SaveLoadOrderMismatched() override = default;
};

class SerializationError_SerializeMethodNotFound : public SerializationError {

public:
    void ToString(std::vector<std::string>& output) const override;

    SerializationError_SerializeMethodNotFound(SerializableClassInfoPtr owningClass);

    ~SerializationError_SerializeMethodNotFound() override = default;
};