#include "SerializableFieldInfo.h"
#include "SerializeOperationInfo.h"

bool SerializableFieldInfo::operator==(const SerializeOperationInfo& field) {
    return GetName() == field.GetName();
}