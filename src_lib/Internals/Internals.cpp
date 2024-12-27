#include "Internals.h"
#include <PostProcessing/SerializableClassInfo.h>
#include <PostProcessing/SerializeFunctionInfo.h>
#include "SerializationContext.h"
#include <PostProcessing/SerializableFieldInfo.h>
#include <PostProcessing/SerializeOperationInfo.h>
#include "clang/AST/DeclTemplate.h"

FindSerializableClassVisitor::FindSerializableClassVisitor(ASTContext *Context)
    : Context(Context) {
    
}

/// @brief Visitation of a class, for checking serializable.
/// Phase: Discovery
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
        // No intrusive serialize method exists. One may be set during mediation.
    }
    else {
        //thisClassInfo->GenerateSerializeMethodInfo();
        thisClassInfo->SetSerializeMethodInfo(std::make_shared<SerializeFunctionInfo_Intrusive>(serializeMethod));
    }

    SerializableClassInfoMediator::AddSerializableDecl(Declaration, std::move(thisClassInfo));
    return true;
}

/// @brief Visits free functions to check for whether or not they are a serialize-function.
/// Phase: Discovery
/// @param Declaration - AST type for a specific class.
/// @return true indicates that iteration through the translation unit shall continue
bool FindSerializableClassVisitor::VisitFunctionDecl(FunctionDecl *FuncDecl) {
    if (FuncDecl->getNameAsString() != "serialize" || !FuncDecl->isTemplateDecl()) {
        return true;
    }
    auto asTemplateDecl = dynamic_cast<FunctionTemplateDecl*>(FuncDecl);
    if (FuncDecl->getNumParams() < 2) { // Free serialize methods have 3 parameters, second is type
        return true;
    }
    
    auto asTemplateDecl_Ptr = std::make_shared<SerializeFunctionInfo_NonIntrusive>(asTemplateDecl);
    
    auto body = asTemplateDecl->getBody();

    SerializableStmtVisitor visitor{};
    visitor.TraverseStmt(body);
    auto methodContents = visitor.GetSerializeContents();

    for (auto& field : methodContents) {
        asTemplateDecl_Ptr->AddSerializableField(SerializeOperationInfo(field));
    }

    SerializeFunctionInfoMediator::AddSerializeDecl(asTemplateDecl, std::move(asTemplateDecl_Ptr));
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

    SerializationContext::Mediate();

    SerializationContext::Analyze();
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