#include "SerializeFunctionInfo.h"

SerializeFunctionInfo::SerializeFunctionInfo(clang::FunctionTemplateDecl* decl) : m_decl{decl} {
    
}

SerializeFunctionInfo_Intrusive::SerializeFunctionInfo_Intrusive(clang::FunctionTemplateDecl* decl) : SerializeFunctionInfo(decl) {

}

SerializeFunctionInfo_NonIntrusive::SerializeFunctionInfo_NonIntrusive(clang::FunctionTemplateDecl* decl) : SerializeFunctionInfo(decl)  {
    
}