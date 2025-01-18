#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <vector>

// ==================================
// Internal Headers
// ==================================
#include <Types/InfoTypes.h>
#include <SerializationInfo/SerializationErrors.h>
#include <SerializationInfo/SerializationErrors.h>

// ==================================
// Libtooling Headers
// ==================================
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/AST.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

// ==================================
// Forward Declarations
// ==================================


// Mediator-classes store both the AST information and the persistent information. They exist until mediation phase.

// Relates CXXRecordDecl's to Info-type
class SerializableClassInfoMediator {
public:
    friend class SerializationContext;

    static bool AddSerializableDecl(clang::CXXRecordDecl* decl, SerializableClassInfoPtr&& serializable);

    static std::vector<SerializableClassInfoWeakPtr> FlattenSerializableContainer();

    static void Reset();
private:
    static std::unordered_map<std::string, SerializableClassInfoPtr> serializables;
};

// Relates serialize methods to Info-type
class SerializeFunctionInfoMediator {
public:
    friend class SerializationContext;

    static bool AddSerializeDecl(clang::FunctionDecl* decl, SerializeFunctionInfoPtr&& serializable);

    static void Reset();
private:
    static std::unordered_map<std::string, SerializeFunctionInfoPtr> serializables;
};

class DiscoveryHelper {
public:
    static bool FetchSerializeMethod(clang::ASTContext& context, const clang::CXXRecordDecl* serializable, /* out */ clang::FunctionTemplateDecl*& serializeDecl);
    static void FetchSerializableMembers(clang::ASTContext& context, const clang::CXXRecordDecl* serializable, SerializableClassInfoPtr classInfo);
    static bool IsSerializationSplit(clang::ASTContext& context, const clang::Decl* serializable, SerializableClassInfoPtr classInfo);
};