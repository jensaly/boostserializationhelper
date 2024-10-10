#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

class FindSerializableFieldsVisitor
  : public clang::RecursiveASTVisitor<FindSerializableFieldsVisitor> {
public:
  explicit FindSerializableFieldsVisitor(ASTContext *Context)
    : Context(Context) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    for (auto field : Declaration->fields()) {
      if (field->hasAttr<AnnotateAttr>()) {
        llvm::outs() <<field->getAttr<AnnotateAttr>()->getAnnotation() << "\n";
      }
      llvm::outs() << field->getNameAsString() << "\n";
      
    }
    return true;
  }
private:
  ASTContext *Context;
};

class FindSerializableClassConsumer : public clang::ASTConsumer {
public:
  explicit FindSerializableClassConsumer(clang::ASTContext *Context)
    : Visitor(Context) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
private:
  // A RecursiveASTVisitor implementation.
  FindSerializableFieldsVisitor Visitor;
};

class FindSerializableClassAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::make_unique<FindSerializableClassConsumer>(&Compiler.getASTContext());
  }
};