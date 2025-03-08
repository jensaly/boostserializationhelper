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
        : m_fieldName{field.GetName()},
        m_filenameDecl{field.GetFilename()}, m_declLine{field.GetLine()}, m_declColumn{field.GetColumn()},
        m_filenameSave{method.GetFilename()}, m_saveLine{method.GetLine()}, m_saveColumn{method.GetColumn()},
        SerializationError(SerializationErrorFlag::Error_MarkedFieldNotSerialized, owningClass)
{
    m_split = false;
}

void SerializationError_MarkedFieldNotSerialized::ToString(std::vector<std::string>& output) const {
    constexpr std::string_view format = 
        "[E] {flag}: Marked field '{field}' in '{class}' (declared in '{declFile}', {declPos}) is not present in 'serialize'-method (defined in '{saveFile}', {savePos})";

    auto classPtr = m_class.lock();
    // TODO: Check expired!

    std::string declPos = fmt::format("({}, {})", m_declLine, m_declColumn);
    std::string savePos = fmt::format("({}, {})", m_saveLine, m_saveColumn);
    
    output.push_back(fmt::format(format, 
        fmt::arg("flag", toStringSep(m_flag)),
        fmt::arg("field", m_fieldName),
        fmt::arg("class", classPtr->GetClassName()),
        fmt::arg("declFile", m_filenameDecl),
        fmt::arg("declPos", declPos),
        fmt::arg("saveFile", m_filenameSave),
        fmt::arg("savePos", savePos)
    ));
}

SerializationError_UnmarkedFieldSerialized::SerializationError_UnmarkedFieldSerialized(
    SerializeOperationInfo& operation, SerializeFunctionInfo& function, SerializableClassInfoPtr owningClass) 
        : SerializationError(SerializationErrorFlag::Error_UnmarkedFieldSerialized, owningClass)
{
    m_fieldName = operation.GetName();
    m_filenameDecl = operation.GetFilename();
    m_declLine = operation.GetLine();
    m_declColumn = function.GetColumn();
    m_filenameSave = operation.GetOperationFilename();
    m_saveLine = operation.GetOperationLine();
    m_saveColumn = operation.GetOperationColumn();
    // TODO: Find the line for the field decl in the class definition, that is out filenameDecl and declLine
}

void SerializationError_UnmarkedFieldSerialized::ToString(std::vector<std::string>& output) const {
    constexpr std::string_view format = 
        "[E] {flag}: Unmarked field '{field}' in {class} ('{declFile}', {declPos}) is present in the 'serialize'-method ('{saveFile}', {savePos})";

    auto classPtr = m_class.lock();
    // TODO: Check expired!

    std::string declPos = fmt::format("({}, {})", m_declLine, m_declColumn);
    std::string savePos = fmt::format("({}, {})", m_saveLine, m_saveColumn);
    output.push_back(fmt::format(format, 
        fmt::arg("flag", toStringSep(m_flag)),
        fmt::arg("field", m_fieldName),
        fmt::arg("class", classPtr->GetClassName()),
        fmt::arg("declFile", m_filenameDecl),
        fmt::arg("declPos", declPos),
        fmt::arg("saveFile", m_filenameSave),
        fmt::arg("savePos", savePos)
    ));
}

SerializationError_SaveLoadOrderMismatched::SerializationError_SaveLoadOrderMismatched(
    SerializeOperationInfo& loadOperation, SerializeOperationInfo& saveOperation, SerializeFunctionInfo& saveFunction, SerializeFunctionInfo& loadFunction, SerializableClassInfoPtr owningClass) 
        : SerializationError(SerializationErrorFlag::Error_SaveLoadOrderMismatched, owningClass)
{
    (void)saveFunction;
    (void)loadFunction;
    (void)owningClass;
    m_fieldName = loadOperation.GetName();
    m_filenameLoad = loadOperation.GetFilename();
    m_loadLine = loadOperation.GetLine();
    m_loadColumn = loadOperation.GetColumn();

    m_filenameSave = saveOperation.GetFilename();
    m_saveLine = saveOperation.GetLine();
    m_saveColumn = saveOperation.GetColumn();
}

void SerializationError_SaveLoadOrderMismatched::ToString(std::vector<std::string>& output) const {
    constexpr std::string_view format = 
        "[E] {flag}: Mistmatch in save/load-order for '{field}' in {class}. Save operation: '{saveFile}', {savePos}. Load operation: ('{loadFile}', {loadPos})";

    auto classPtr = m_class.lock();
    // TODO: Check expired!

    std::string loadPos = fmt::format("({}, {})", m_loadLine, m_loadColumn);
    std::string savePos = fmt::format("({}, {})", m_saveLine, m_saveColumn);
    output.push_back(fmt::format(format, 
        fmt::arg("flag", toStringSep(m_flag)),
        fmt::arg("field", m_fieldName),
        fmt::arg("class", classPtr->GetClassName()),
        fmt::arg("saveFile", m_filenameSave),
        fmt::arg("savePos", savePos),
        fmt::arg("loadFile", m_filenameLoad),
        fmt::arg("loadPos", loadPos)
    ));
}

SerializationError_SerializeMethodNotFound::SerializationError_SerializeMethodNotFound(SerializableClassInfoPtr owningClass) 
        : SerializationError(SerializationErrorFlag::Error_SerializeMethodNotFound, owningClass)
{

}

void SerializationError_SerializeMethodNotFound::ToString(std::vector<std::string>& output) const {
    constexpr std::string_view format = "[E] {flag}: Serializable class '{class}' (declared on '{declFile}', {declPos}) does not have a 'serialize'-method";

    auto classPtr = m_class.lock();
    // TODO: Check expired!

    std::string declPos = fmt::format("({}, {})", classPtr->GetLine(), classPtr->GetColumn());
    output.push_back(fmt::format(format, 
        fmt::arg("flag", toStringSep(m_flag)),
        fmt::arg("class", classPtr->GetClassName()),
        fmt::arg("declFile", classPtr->GetFilename()),
        fmt::arg("declPos", declPos)
    ));
}