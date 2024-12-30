// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <Internals/Internals.h>
#include <SerializationInfo/InfoFactory.h>
#include <SerializationInfo/SerializableClassInfo.h>
#include <SerializationInfo/SerializeFunctionInfo.h>
#include <SerializationInfo/SerializableFieldInfo.h>
#include <SerializationInfo/SerializeOperationInfo.h>
#include <SerializationContext.h>
#include <Utils/Utils.h>

// ==================================
// Libtooling Headers
// ==================================
#include "clang/AST/DeclTemplate.h"

// ==================================
// Forward Declarations
// ==================================

using namespace clang;

FindSerializableClassVisitor::FindSerializableClassVisitor(ASTContext *Context)
    : Context(Context) {
    
}

/// @brief Visitation of a class, for checking serializable.
/// Phase: Discovery
/// @param Declaration - AST type for a specific class.
/// @return true indicates that iteration through the translation unit shall continue
bool FindSerializableClassVisitor::VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    if (!isClassSerializable(Declaration)) {
        return true;
    }
    
    auto thisClassInfo = InfoFactory::Create<SerializableClassInfo>(Context, Declaration);
    
    DiscoveryHelper::FetchSerializableMembers(*Context, Declaration, thisClassInfo);

    FunctionTemplateDecl* serializeMethodTemplate = nullptr;
    if (!DiscoveryHelper::FetchSerializeMethod(*Context, Declaration, serializeMethodTemplate)) {
        // No intrusive serialize method exists. One may be set during mediation.
    }
    else {
        auto serializeMethod = serializeMethodTemplate->getAsFunction();
        // Intrusive method found. Set it.
        auto intrusiveSerializeMethodInfo = std::make_shared<SerializeFunctionInfo_Intrusive>(serializeMethod);
        auto body = serializeMethod->getBody();

        SerializableStmtVisitor visitor{Context};
        visitor.TraverseStmt(body);
        auto methodContents = visitor.GetOperations();

        for (auto& field : methodContents) {
            intrusiveSerializeMethodInfo->AddSerializableField(SerializeOperationInfo(field));
        };

        thisClassInfo->SetSerializeMethodInfo(intrusiveSerializeMethodInfo);
    }

    SerializableClassInfoMediator::AddSerializableDecl(Declaration, std::move(thisClassInfo));
    return true;
}

/// @brief Visits free functions to check for whether or not they are a serialize-function.
/// Phase: Discovery
/// @param Declaration - AST type for a specific class.
/// @return true indicates that iteration through the translation unit shall continue
bool FindSerializableClassVisitor::VisitFunctionDecl(FunctionDecl *FuncDecl) {
    if (FuncDecl->getNameAsString() != "serialize") {
        return true;
    }

    if (FuncDecl->getDeclContext()->isRecord()) {
        return true;
    }

    // Check that it is a template function?
    if (FuncDecl->getNumParams() < 2) { // Free serialize methods have 3 parameters, second is type
        return true;
    }
    
    auto FuncDecl_Ptr = std::make_shared<SerializeFunctionInfo_NonIntrusive>(FuncDecl);
    
    auto body = FuncDecl->getBody();

    SerializableStmtVisitor visitor{};
    visitor.TraverseStmt(body);
    auto methodContents = visitor.GetSerializeContents();

    for (auto& field : methodContents) {
        FuncDecl_Ptr->AddSerializableField(SerializeOperationInfo(field));
    }

    SerializeFunctionInfoMediator::AddSerializeDecl(FuncDecl, std::move(FuncDecl_Ptr));
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

SerializableStmtVisitor::SerializableStmtVisitor(clang::ASTContext* context)
        : Context{context}
{

}

bool SerializableStmtVisitor::VisitBinaryOperator(const BinaryOperator *op) {
    if (op->getOpcode() == BO_And) {
        const Expr* lhs = op->getLHS();
        const Expr* rhs = op->getRHS();
        if (const auto rhs_decl = dyn_cast<MemberExpr>(rhs)) {
            auto operatonInfo = InfoFactory::Create<SerializeOperationInfo>(Context, rhs_decl->getMemberDecl());
            m_serializeContents.push_back(std::move(operatonInfo));
        }
    }
    return true;
}

const std::vector<SerializeOperationInfoPtr> SerializableStmtVisitor::GetOperations() {
    return m_serializeContents;
}