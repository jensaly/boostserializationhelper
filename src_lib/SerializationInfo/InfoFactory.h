#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <vector>
#include <memory>

// ==================================
// Internal Headers
// ==================================

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
    static std::shared_ptr<Output> Create(clang::ASTContext& context, clang::Decl* Declaration) {
        auto name = Declaration->getNameAsString();
        std::string filename;
        unsigned int line, column;
        Utils::GetFullLocaionOfDecl(Context, Declaration, filename, line, column);

        auto thisClassInfo = std::make_shared<Output>(name, filename, line, column);
    }
};