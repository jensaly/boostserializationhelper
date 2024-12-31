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
};