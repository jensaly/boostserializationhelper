#pragma once

// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================

// ==================================
// Libtooling Headers
// ==================================
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ASTContext.h"

// ==================================
// Forward Declarations
// ==================================

class Utils {
public:
    static bool GetFullLocaionOfDecl(clang::ASTContext& context, clang::Decl* decl, /* out */ std::string& filename, /* out */ unsigned int& line, /* out */ unsigned int& column);

    static bool GetFullLocaionOfExpr(clang::ASTContext& context, const clang::MemberExpr* decl, /* out */ std::string& filename, /* out */ unsigned int& line, /* out */ unsigned int& column);
};