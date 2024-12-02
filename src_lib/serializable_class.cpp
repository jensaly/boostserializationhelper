#include "serializable_class.h"

FindSerializableClassVisitor::FindSerializableClassVisitor(ASTContext *Context)
    : Context(Context) {
    
}

/// @brief 
/// @param Declaration 
/// @return true indicates that iteration through the translation unit shall continue
bool FindSerializableClassVisitor::VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    auto name = Declaration->getNameAsString();
    if (!isClassSerializable(Declaration)) {
        return true;
    }
    
    if (Declaration->isCompleteDefinition())
        SerializableCXXRecordDeclStorage::AddSerializableDecl(Declaration->getCanonicalDecl());
    /*
    for (auto field : Declaration->fields()) {
        if (field->hasAttr<AnnotateAttr>()) {
        llvm::outs() <<field->getAttr<AnnotateAttr>()->getAnnotation() << "\n";
        }
        llvm::outs() << field->getNameAsString() << "\n";
    }
    */
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

    llvm::outs() << ((SerializableCXXRecordDeclStorage::GetClasses()[0], "Basic_SerializableWithoutFunction") ? "HasAnnotation" : "HasNotAnnotation");

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