#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

// Visitor class to walk through the AST
class SerializationVisitor : public RecursiveASTVisitor<SerializationVisitor> {
public:
    explicit SerializationVisitor(ASTContext *Context)
        : Context(Context) {}

    bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
        if (Declaration->hasDefinition()) {
            llvm::outs() << "Class: " << Declaration->getNameAsString() << "\n";

            // Collect all fields marked with "serializable"
            for (auto *Field : Declaration->fields()) {
                if (Field->hasAttr<AnnotateAttr>() && Field->getAttr<AnnotateAttr>()->getAnnotation() == "serializable") {
                    llvm::outs() << "  Serializable field: " << Field->getNameAsString() << "\n";
                    // You would then check if this field is serialized in the `serialize()` method.
                }
            }
        }
        return true;
    }

private:
    ASTContext *Context;
};
