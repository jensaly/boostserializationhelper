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

SerializeFunctionInfo::SerializeFunctionInfo(clang::FunctionDecl* decl) : m_decl{decl} {
    
}

SerializeFunctionInfo::~SerializeFunctionInfo() {

}

void SerializeFunctionInfo::AddSerializableField(SerializeOperationInfo operationInfo) {
    m_operationsInfo.push_back(operationInfo);
}

void SerializeFunctionInfo::SetError(SerializationErrorFlag error) {
    m_errorFlags |= error;
}

bool SerializeFunctionInfo::HasError(SerializationErrorFlag error) const {
    if (error != SerializationErrorFlag::Error_NoError) 
        return (m_errorFlags & error) != SerializationErrorFlag::Error_NoError;
    return true;
}

SerializeFunctionInfo_Intrusive::SerializeFunctionInfo_Intrusive(clang::FunctionDecl* decl) : SerializeFunctionInfo(decl) {

}

SerializeFunctionInfo_NonIntrusive::SerializeFunctionInfo_NonIntrusive(clang::FunctionDecl* decl) : SerializeFunctionInfo(decl)  {
    
}