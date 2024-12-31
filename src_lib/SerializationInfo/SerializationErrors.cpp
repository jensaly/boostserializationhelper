// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializationErrors.h>
#include <SerializationInfo/SerializableFieldInfo.h>
#include <SerializationInfo/SerializeOperationInfo.h>
#include <SerializationInfo/SerializeFunctionInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

// For normal serialization
SerializationError_MarkedFieldNotSerialized::SerializationError_MarkedFieldNotSerialized(
    SerializableFieldInfo& field, SerializeFunctionInfo& method)
        : m_filenameDecl{field.GetFilename()}, m_declLine{field.GetLine()}, m_declColumn{field.GetColumn()},
        m_filenameSave{method.GetFilename()}, m_saveLine{method.GetLine()}, m_saveColumn{method.GetColumn()},
        SerializationError(SerializationErrorFlag::Error_MarkedFieldNotSerialized)
{
    m_split = false;
}

std::string SerializationError_MarkedFieldNotSerialized::ToString() const {
    throw std::logic_error("Not implemented");
}

SerializationError_UnmarkedFieldSerialized::SerializationError_UnmarkedFieldSerialized(
    SerializeOperationInfo& operation, SerializeFunctionInfo& function) 
        : SerializationError(SerializationErrorFlag::Error_UnmarkedFieldSerialized)
{
    m_declLine = function.GetLine();
    m_saveLine = operation.GetLine();
    m_declColumn = function.GetColumn();
    m_saveColumn = operation.GetColumn();
}

std::string SerializationError_UnmarkedFieldSerialized::ToString() const {
    throw std::logic_error("Not implemented");
}

SerializationError_SerializeMethodNotFound::SerializationError_SerializeMethodNotFound() 
        : SerializationError(SerializationErrorFlag::Error_SerializeMethodNotFound)
{

}

std::string SerializationError_SerializeMethodNotFound::ToString() const {
    throw std::logic_error("Not implemented");
}