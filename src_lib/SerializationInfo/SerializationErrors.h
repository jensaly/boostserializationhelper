#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <cstdint>

// ==================================
// Internal Headers
// ==================================

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
public:
    SerializationErrorFlag m_flag = SerializationErrorFlag::Error_NoError;
    SerializationError() = default;

    // Used to create errors noe tied to a specific location
    SerializationError(SerializationErrorFlag flag)
            : m_flag{flag} {}

    virtual ~SerializationError() {}
    
    SerializationErrorFlag GetFlag() const { return m_flag; }

    virtual std::string ToString() const = 0;
};

class SerializationError_MarkedFieldNotSerialized : public SerializationError {
private:
    std::string m_filenameDecl = "";
    unsigned int m_declLine; // Max value indicates that this error isn't related to a specific line
    unsigned int m_declColumn; // Max value indicates that this error isn't related to a specific column
    std::string m_filenameSave = "";
    unsigned int m_saveLine;
    unsigned int m_saveColumn;
    bool m_split = false; // If set to false, there's a single serialize-function. 

public:
    // For split-member serialization
    //SerializationError_MarkedFieldNotSerialized(SerializeFie);
    
    // For normal serialization
    SerializationError_MarkedFieldNotSerialized(SerializableFieldInfo& field, SerializeFunctionInfo& method);

    std::string ToString() const override;

    ~SerializationError_MarkedFieldNotSerialized() override = default;
};

class SerializationError_UnmarkedFieldSerialized : public SerializationError {
private:
    std::string m_filenameDecl = "";
    unsigned int m_declLine; // Max value indicates that this error isn't related to a specific line
    unsigned int m_declColumn; // Max value indicates that this error isn't related to a specific column
    std::string m_filenameSave = "";
    unsigned int m_saveLine;
    unsigned int m_saveColumn;
    bool m_split = false; // If set to false, there's a single serialize-function. 

public:
    // For split-member serialization
    SerializationError_UnmarkedFieldSerialized(SerializeOperationInfo& operation, SerializeFunctionInfo& function);

    std::string ToString() const override;

    ~SerializationError_UnmarkedFieldSerialized() override = default;
};

class SerializationError_SerializeMethodNotFound : public SerializationError {

public:
    std::string ToString() const override;

    SerializationError_SerializeMethodNotFound();

    ~SerializationError_SerializeMethodNotFound() override = default;
};