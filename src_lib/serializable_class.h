#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "serializable_class_storage.h"

using namespace clang;

class FindSerializableClassVisitor
  : public clang::RecursiveASTVisitor<FindSerializableClassVisitor> {
public:
  explicit FindSerializableClassVisitor(ASTContext *Context);
  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration);
private:
  ASTContext *Context;

  bool isClassSerializable(CXXRecordDecl *declaration);
};

class FindSerializableClassConsumer : public clang::ASTConsumer {
public:
  explicit FindSerializableClassConsumer(clang::ASTContext *Context);
  virtual void HandleTranslationUnit(clang::ASTContext &Context);
private:
  // A RecursiveASTVisitor implementation.
  FindSerializableClassVisitor Visitor;
};

class FindSerializableClassAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile);
};