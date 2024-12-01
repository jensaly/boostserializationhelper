#include <bitset>
#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

enum class WarningType {
    MissingSerializeMethod,
    SerializableMemberNotInFunction,
    SerializationOrderMismatched,
};

using WarningFlags = std::bitset<64>;

struct DeclAnalysisResult {
    CXXRecordDecl* decl;
    WarningFlags warnings;

    void setWarning(WarningType type) { warnings.set(static_cast<size_t>(type)); }
    bool hasWarning(WarningType type) const { return warnings.test(static_cast<size_t>(type)); }
};