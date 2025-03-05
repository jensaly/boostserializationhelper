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
    
    auto thisClassInfo = InfoFactory::Create<SerializableClassInfo>(*Context, Declaration);
    
    DiscoveryHelper::FetchSerializableMembers(*Context, Declaration, thisClassInfo);

    // Two separate paths for whether the class is split-serialized or not.

    FunctionTemplateDecl* serializeMethodTemplate = nullptr;
    if (!DiscoveryHelper::FetchSerializeMethod(*Context, Declaration, thisClassInfo)) {
        // No intrusive serialize method exists. One may be set during mediation.
    }
    else {
        // Intrusive serialize or save/load methods found.
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

    auto FuncDecl_Ptr = InfoFactory::Create<SerializeFunctionInfo_NonIntrusive>(*Context, FuncDecl);
    
    auto body = FuncDecl->getBody();

    SerializableStmtVisitor visitor{Context};
    visitor.TraverseStmt(body);
    auto methodContents = visitor.GetOperations();

    for (auto& field : methodContents) {
        FuncDecl_Ptr->AddSerializableField(std::move(field));
    }
    // methodContents now useless.

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

    SerializationContext::Log();
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
    auto operation = op->getOpcode();
    // TODO: Replace with a switch
    // TODO: Nullptr check, shall it always be bad?
    const Expr* lhs = op->getLHS();
    const Expr* rhs = op->getRHS();
    if (lhs == nullptr || rhs == nullptr) {
            
    }
    // TODO: Check that lhs is the archive, otherwise this isn't a serialization operation
    //if (lhs != ar)

    SerializeOperationInfoPtr operationInfo;
    if (operation == BO_And) {
        // Operation uses &
        if (const auto rhs_decl = dyn_cast<MemberExpr>(rhs)) {
            operationInfo = InfoFactory::Create<SerializeOperationInfo>(*Context, rhs_decl);
            operationInfo->SetBinaryOperatorKind(BinaryOperatorKind::BO_And);
        }
    }
    else if (operation == BinaryOperatorKind::BO_Shl) {
        // Operation is a save directional
        if (const auto rhs_decl = dyn_cast<MemberExpr>(rhs)) {
            operationInfo = InfoFactory::Create<SerializeOperationInfo>(*Context, rhs_decl);
            operationInfo->SetBinaryOperatorKind(BinaryOperatorKind::BO_Shl);
        }
    }
    else if (operation == BinaryOperatorKind::BO_Shr) {
        // Operation is a load directional
        if (const auto rhs_decl = dyn_cast<MemberExpr>(rhs)) {
            operationInfo = InfoFactory::Create<SerializeOperationInfo>(*Context, rhs_decl);
            operationInfo->SetBinaryOperatorKind(BinaryOperatorKind::BO_Shr);
        }
    }
    else {
        // Else we don't care for it.
        return true;
    }
    m_serializeContents.push_back(std::move(operationInfo));
    return true;
}

bool SerializableStmtVisitor::VisitCallExpr(const CallExpr* call) {
    const Expr *CalleeExpr = call->getCallee()->IgnoreImplicit()->IgnoreParenCasts();

    if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(CalleeExpr)) {
        if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
            if (FD->getQualifiedNameAsString() == "boost::serialization::split_member") {
                m_split_internal = true;
                return false;
                // We end here under the assumption that the user does not put any further information in the serialize-function after calling this.
                // TODO: We should probably intoduce a check to see that there is no more content, and raise a warning about bad behavior.
            }
        }
    }
    return true;
}

const std::vector<SerializeOperationInfoPtr> SerializableStmtVisitor::GetOperations() {
    return m_serializeContents;
}

SaveStmtVisitor::SaveStmtVisitor(clang::ASTContext* context)
        : SerializableStmtVisitor(context)
{

}

bool SaveStmtVisitor::VisitBinaryOperator(const BinaryOperator *op) {
    if (op->getOpcode() == BO_And) {
        const Expr* lhs = op->getLHS();
        const Expr* rhs = op->getRHS();
        if (const auto rhs_decl = dyn_cast<MemberExpr>(rhs)) {
            auto operatonInfo = InfoFactory::Create<SerializeOperationInfo>(*Context, rhs_decl);
            m_serializeContents.push_back(std::move(operatonInfo));
        }
    }
    return true;
}

