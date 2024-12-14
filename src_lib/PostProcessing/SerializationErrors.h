#include <cstdint>

enum class SerializationError : uint64_t {
    Error_NoError = 0,
    Error_MarkedFieldNotSerialized = 1 << 0, // A field marked for serialization is not present in the serialize/save/load method
    Error_UnmarkedFieldSerialized = 1 << 1, // A field not marked for serialization is present in the serialize method
    Error_SerializeMethodNotFound = 1 << 2, // No serialize-method is found (TODO: Reconsider, the compile may handle this)
    Error_SaveLoadOrderMismatched = 1 << 3 // In the split save-load methods, the order of operations is not the same
};

enum class SerializationInformation : uint64_t {
    Info_NoInfo = 0,
    Info_NonIntrusive = 1 << 0, // The class's serialization functionality is non-intrusive.
    Info_SplitSerialization = 1 << 1, // The class uses a split save/load
};

inline SerializationError operator|(SerializationError lhs, SerializationError rhs) {
    return static_cast<SerializationError>(
        static_cast<uint64_t>(lhs) | static_cast<uint64_t>(rhs)
    );
}

inline SerializationError& operator|=(SerializationError& lhs, SerializationError rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline SerializationError operator&(SerializationError lhs, SerializationError rhs) {
    return static_cast<SerializationError>(
        static_cast<uint64_t>(lhs) & static_cast<uint64_t>(rhs)
    );
}
