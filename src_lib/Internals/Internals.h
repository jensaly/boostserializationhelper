#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include <Mediation/SerializableClassInfoMediator.h>

// AST Traversal and object tracking
// Phase: Discovery.

class FindSerializableClassVisitor
  : public clang::RecursiveASTVisitor<FindSerializableClassVisitor> {
public:
  explicit FindSerializableClassVisitor(clang::ASTContext *Context);
  bool VisitCXXRecordDecl(clang::CXXRecordDecl *Declaration);
  bool VisitFunctionDecl(clang::FunctionDecl *Declaration);
private:
  clang::ASTContext *Context;

  bool isClassSerializable(clang::CXXRecordDecl *declaration);
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

// Visitor for the Stmt of serialize functions
class SerializableStmtVisitor : public clang::RecursiveASTVisitor<SerializableStmtVisitor> {
  std::vector<SerializeOperationInfoPtr> m_serializeContents;
  bool m_split_internal{false};
  clang::ASTContext *Context;
public:
  explicit SerializableStmtVisitor(clang::ASTContext* context);
  virtual bool VisitBinaryOperator(const clang::BinaryOperator *op);
  bool VisitCallExpr(const  clang::CallExpr* call);
  const std::vector<SerializeOperationInfoPtr> GetOperations();
  bool IsSplitInternal() const { return m_split_internal; }
};

class SaveStmtVisitor : public SerializableStmtVisitor{
  std::vector<SerializeOperationInfoPtr> m_serializeContents;
  clang::ASTContext *Context;
public:
  explicit SaveStmtVisitor(clang::ASTContext* context);
  bool VisitBinaryOperator(const clang::BinaryOperator *op) override;
  const std::vector<SerializeOperationInfoPtr> GetOperations();
};

class LoadStmtVisitor : public SerializableStmtVisitor{
  std::vector<SerializeOperationInfoPtr> m_serializeContents;
  clang::ASTContext *Context;
public:
  explicit LoadStmtVisitor(clang::ASTContext* context);
  bool VisitBinaryOperator(const clang::BinaryOperator *op) override;
  const std::vector<SerializeOperationInfoPtr> GetOperations();
};