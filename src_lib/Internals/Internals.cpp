#include "Internals.h"
#include <PostProcessing/SerializableClassInfo.h>
#include <PostProcessing/SerializeFunctionInfo.h>
#include "clang/AST/DeclTemplate.h"

FindSerializableClassVisitor::FindSerializableClassVisitor(ASTContext *Context)
    : Context(Context) {
    
}

/// @brief 
/// @param Declaration - AST type for a specific class.
/// @return true indicates that iteration through the translation unit shall continue
bool FindSerializableClassVisitor::VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    auto name = Declaration->getNameAsString();
    if (!isClassSerializable(Declaration)) {
        return true;
    }
    
    auto thisClassInfo = std::make_shared<SerializableClassInfo>(name);
    
    ClassAnalyzer::FetchSerializableMembers(Declaration, thisClassInfo);

    FunctionTemplateDecl* serializeMethod = nullptr;
    if (!ClassAnalyzer::FetchSerializeMethod(Declaration, serializeMethod)) {
        thisClassInfo->SetError(SerializationError::Error_SerializeMethodNotFound);
    }
    else {
        thisClassInfo->GenerateSerializeMethodInfo();
        ClassAnalyzer::AnalyzeSerializeMethod(serializeMethod, thisClassInfo);
    }

    SerializableClassInfoAggregator::AddSerializableDecl(std::move(thisClassInfo));
    return true;
}

/// @brief Visits free functions
/// @param Declaration - AST type for a specific class.
/// @return true indicates that iteration through the translation unit shall continue
bool FindSerializableClassVisitor::VisitFunctionDecl(FunctionDecl *FuncDecl) {
    if (FuncDecl->getNameAsString() == "serialize" && FuncDecl->isTemplateDecl()) {
        auto asTemplateDecl = dynamic_cast<FunctionTemplateDecl*>(FuncDecl);
        if (FuncDecl->getNumParams() > 1) { // Free serialize methods have 3 parameters, second is type
            QualType ParamType = FuncDecl->getParamDecl(1)->getType();
            auto asTemplateDecl_Ptr = std::make_shared<SerializeFunctionInfo_NonIntrusive>(asTemplateDecl);
            // Look up the CXXRecordDecl and (if one has been discovered) associate it via weak_ptr
            SerializeFunctionInfoAggregator::AddSerializeDecl(std::move(asTemplateDecl_Ptr));
        }
    }
    return true;
}

bool FindSerializableClassVisitor::isClassSerializable(CXXRecordDecl *declaration) {
    if (!declaration->hasAttr<AnnotateAttr>()) {
        return false;
    }

    auto attr = declaration->getAttr<AnnotateAttr>();

    return attr->getAnnotation() == "serializable";
}

FindSerializableClassConsumer::FindSerializableClassConsumer(clang::ASTContext *Context) : Visitor(Context) {

}

void FindSerializableClassConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());    
}

std::unique_ptr<clang::ASTConsumer> FindSerializableClassAction::CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    auto ptr = std::make_unique<FindSerializableClassConsumer>(&Compiler.getASTContext());

    return std::move(ptr);
}

bool SerializableStmtVisitor::VisitBinaryOperator(const BinaryOperator *op) {
    if (op->getOpcode() == BO_And) {
        const Expr* lhs = op->getLHS();
        const Expr* rhs = op->getRHS();
        
        if (const auto rhs_decl = dyn_cast<MemberExpr>(rhs)) {
            m_serializeContents.push_back(rhs_decl->getMemberDecl()->getNameAsString());
        }
    }
    return true;
}

const std::vector<std::string> SerializableStmtVisitor::GetSerializeContents() {
    return m_serializeContents;
}