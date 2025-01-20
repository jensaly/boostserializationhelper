#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <vector>
#include <memory>

// ==================================
// Internal Headers
// ==================================
#include <Utils/Utils.h>

// ==================================
// Libtooling Headers
// ==================================
#include "clang/AST/Decl.h"
#include "clang/AST/ASTContext.h"

// ==================================
// Forward Declarations
// ==================================

class InfoFactory {
public:
    template<class Output>
    static std::shared_ptr<Output> Create(clang::ASTContext& context, clang::NamedDecl* Declaration) {
        auto name = Declaration->getNameAsString();
        std::string filename;
        unsigned int line, column;
        Utils::GetFullLocaionOfDecl(context, Declaration, filename, line, column);

        return std::make_shared<Output>(name, filename, line, column);
    }

    template<class Output>
    static std::shared_ptr<Output> Create(clang::ASTContext& context, const clang::MemberExpr* expr) {
        // exprName will be either [object variable name].expr or just expr.
        std::string exprName = expr->getMemberNameInfo().getAsString();
        std::string exprFileName;
        unsigned int exprLine, exprColumn;
        Utils::GetFullLocaionOfExpr(context, expr, exprFileName, exprLine, exprColumn);
        
        auto decl = expr->getMemberDecl();
        auto declName = decl->getNameAsString();
        std::string declFileName;
        unsigned int declLine, declColumn;
        Utils::GetFullLocaionOfDecl(context, decl, declFileName, declLine, declColumn);

        return std::make_shared<Output>(exprName, exprName, exprLine, exprColumn,
                declName, declFileName, declLine, declColumn
        );
    }
};