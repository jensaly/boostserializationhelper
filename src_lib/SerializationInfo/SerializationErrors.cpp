// ==================================
// Standard Library Headers & Other Libs
// ==================================
#include <string>
#include <string_view>
#include <fmt/format.h>

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializationErrors.h>
#include <SerializationInfo/SerializableFieldInfo.h>
#include <SerializationInfo/SerializeOperationInfo.h>
#include <SerializationInfo/SerializableClassInfo.h>
#include <SerializationInfo/SerializeFunctionInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

std::string toStringSep(SerializationErrorFlag flag) {
    switch (flag) {
        case SerializationErrorFlag::Error_NoError:
            return "No Error";
        case SerializationErrorFlag::Error_MarkedFieldNotSerialized:
            return "Marked Field Not Serialized";
        case SerializationErrorFlag::Error_UnmarkedFieldSerialized:
            return "Unmarked Field Serialized";
        case SerializationErrorFlag::Error_SerializeMethodNotFound:
            return "Serialize Method Not Found";
        case SerializationErrorFlag::Error_SaveLoadOrderMismatched:
            return "Save-Load Order Mismatched";
        default:
            return "Unknown Error";
    }
}

// For normal serialization
SerializationError_MarkedFieldNotSerialized::SerializationError_MarkedFieldNotSerialized(
    SerializableFieldInfo& field, SerializeFunctionInfo& method, SerializableClassInfoPtr owningClass)
        : m_filenameDecl{field.GetFilename()}, m_declLine{field.GetLine()}, m_declColumn{field.GetColumn()},
        m_filenameSave{method.GetFilename()}, m_saveLine{method.GetLine()}, m_saveColumn{method.GetColumn()},
        SerializationError(SerializationErrorFlag::Error_MarkedFieldNotSerialized, owningClass)
{
    m_split = false;
}

void SerializationError_MarkedFieldNotSerialized::ToString(std::vector<std::string>& output) const {
    constexpr std::string_view format = "[E] {}: Marked field in {} (on {},{}) is not present in serialize function (declaration on {},{})\n";
    output.push_back(fmt::format(format, toStringSep(m_flag), m_class.lock()->GetClassName(), m_declLine, m_declColumn, m_saveLine, m_saveColumn));
}

SerializationError_UnmarkedFieldSerialized::SerializationError_UnmarkedFieldSerialized(
    SerializeOperationInfo& operation, SerializeFunctionInfo& function, SerializableClassInfoPtr owningClass) 
        : SerializationError(SerializationErrorFlag::Error_UnmarkedFieldSerialized, owningClass)
{
    m_declLine = function.GetLine();
    m_saveLine = operation.GetLine();
    m_declColumn = function.GetColumn();
    m_saveColumn = operation.GetColumn();
}

void SerializationError_UnmarkedFieldSerialized::ToString(std::vector<std::string>& output) const {
    constexpr std::string_view format = "[E] {}: Unmarked field in {} (on {},{}) is present in serialize function (on {},{})\n";
    output.push_back(fmt::format(format, toStringSep(m_flag), m_class.lock()->GetClassName(), m_declLine, m_declColumn, m_saveLine, m_saveColumn));
}

SerializationError_SerializeMethodNotFound::SerializationError_SerializeMethodNotFound(SerializableClassInfoPtr owningClass) 
        : SerializationError(SerializationErrorFlag::Error_SerializeMethodNotFound, owningClass)
{

}

void SerializationError_SerializeMethodNotFound::ToString(std::vector<std::string>& output) const {
    constexpr std::string_view format = "[E] {}: Serializable class {} does not have a ::serialize-method\n";
    output.push_back(fmt::format(format, toStringSep(m_flag), m_class.lock()->GetClassName()));
}