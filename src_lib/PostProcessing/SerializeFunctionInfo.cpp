#include "SerializeFunctionInfo.h"
#include "SerializeOperationInfo.h"

SerializeFunctionInfo::SerializeFunctionInfo(clang::FunctionDecl* decl) : m_decl{decl} {
    
}

SerializeFunctionInfo::~SerializeFunctionInfo() {

}

void SerializeFunctionInfo::AddSerializableField(SerializeOperationInfo operationInfo) {
    m_operationsInfo.push_back(operationInfo);
}

void SerializeFunctionInfo::SetError(SerializationError error) {
    m_errors |= error;
}

bool SerializeFunctionInfo::HasError(SerializationError error) const {
    if (error != SerializationError::Error_NoError) 
        return (m_errors & error) != SerializationError::Error_NoError;
    return true;
}

SerializeFunctionInfo_Intrusive::SerializeFunctionInfo_Intrusive(clang::FunctionDecl* decl) : SerializeFunctionInfo(decl) {

}

SerializeFunctionInfo_NonIntrusive::SerializeFunctionInfo_NonIntrusive(clang::FunctionDecl* decl) : SerializeFunctionInfo(decl)  {
    
}