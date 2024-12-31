// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializeFunctionInfo.h>
#include <SerializationInfo/SerializeOperationInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

SerializeFunctionInfo::SerializeFunctionInfo(std::string className, std::string filename, unsigned int line, unsigned int column)
        : SerializationObject(className, filename, line, column)
{

}

SerializeFunctionInfo::~SerializeFunctionInfo() {

}

void SerializeFunctionInfo::AddSerializableField(SerializeOperationInfoPtr&& operationInfo) {
    m_operationsInfo.push_back(std::move(operationInfo));
}

void SerializeFunctionInfo::SetError(SerializationErrorFlag error) {
    m_errorFlags |= error;
}

bool SerializeFunctionInfo::HasError(SerializationErrorFlag error) const {
    if (error != SerializationErrorFlag::Error_NoError) 
        return (m_errorFlags & error) != SerializationErrorFlag::Error_NoError;
    return true;
}

SerializeFunctionInfo_Intrusive::SerializeFunctionInfo_Intrusive(std::string funcName, std::string filename, unsigned int line, unsigned int column)
        : SerializeFunctionInfo(funcName, filename, line, column)
{

}

SerializeFunctionInfo_NonIntrusive::SerializeFunctionInfo_NonIntrusive(std::string funcName, std::string filename, unsigned int line, unsigned int column)
        : SerializeFunctionInfo(funcName, filename, line, column)
{
    
}