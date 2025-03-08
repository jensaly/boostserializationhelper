// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializableFieldInfo.h>
#include <SerializationInfo/SerializeOperationInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

SerializableFieldInfo::SerializableFieldInfo(std::string name, std::string filename, unsigned int line, unsigned int column)
        : SerializationObject(name, filename, line, column) {

}

bool SerializableFieldInfo::operator==(const SerializeOperationInfo& field) {
    return GetName() == field.GetName();
}