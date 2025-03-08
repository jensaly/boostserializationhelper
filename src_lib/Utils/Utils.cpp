// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <Utils/Utils.h>

// ==================================
// Libtooling Headers
// ==================================
#include "clang/Basic/SourceManager.h"

// ==================================
// Forward Declarations
// ==================================

using namespace clang;

bool Utils::GetFullLocaionOfDecl(ASTContext& context, Decl* decl, /* out */ std::string& filename, /* out */ unsigned int& line, /* out */ unsigned int& column) {
    auto& sm = context.getSourceManager();
    auto ploc = sm.getPresumedLoc(decl->getLocation());
    if (ploc.isInvalid()) {
        // log
        return false;
    }
    filename = ploc.getFilename();
    line = ploc.getLine();
    column = ploc.getColumn();
    return true;
}

bool Utils::GetFullLocaionOfExpr(ASTContext& context, const MemberExpr* expr, /* out */ std::string& filename, /* out */ unsigned int& line, /* out */ unsigned int& column) {
    auto& sm = context.getSourceManager();
    auto ploc = sm.getPresumedLoc(expr->getExprLoc());
    if (ploc.isInvalid()) {
        // log
        return false;
    }
    filename = ploc.getFilename();
    line = ploc.getLine();
    column = ploc.getColumn();
    return true;
}