#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

class FindSerializabledClassVisitor
  : public clang::RecursiveASTVisitor<FindSerializabledClassVisitor> {
public:
  explicit FindSerializabledClassVisitor(ASTContext *Context)
    : Context(Context) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    for (auto method : Declaration->methods()) {
      if (method->getNameAsString() == "serialize") {
        llvm::outs() << Declaration->getNameAsString() << " has serialize method";
        return true;
      }
    }
    return true;

    /*
    if (Declaration->getQualifiedNameAsString() == "n::m::C") {
      FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getBeginLoc());
      if (FullLocation.isValid())
        llvm::outs() << "Found declaration at "
                     << FullLocation.getSpellingLineNumber() << ":"
                     << FullLocation.getSpellingColumnNumber() << "\n";
    }
    */
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
  FindSerializabledClassVisitor Visitor;
};

class FindSerializableClassAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::make_unique<FindSerializableClassConsumer>(&Compiler.getASTContext());
  }
};