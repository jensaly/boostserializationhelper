#pragma once

#include "clang/AST/AST.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "PostProcessing/SerializationErrors.h"

using namespace clang;

class SerializableClassInfo;
class SerializeFunctionInfo;

using SerializableClassName = std::string;
using SerializableClassInfoPtr = std::shared_ptr<SerializableClassInfo>;
using SerializableClassInfoWeakPtr = std::weak_ptr<const SerializableClassInfo>;
using SerializeFunctionInfoPtr = std::shared_ptr<SerializeFunctionInfo>;
using SerializeFunctionInfoWeakPtr = std::weak_ptr<const SerializeFunctionInfo>;

// Mediator-classes store both the AST information and the persistent information. They exist until mediation phase.

class SerializableClassInfoMediator {
public:
    friend class SerializationContext;

    static bool AddSerializableDecl(CXXRecordDecl* decl, SerializableClassInfoPtr&& serializable);

    static std::vector<SerializableClassInfoWeakPtr> FlattenSerializableContainer();

    static void Reset();
private:
    static std::unordered_map<std::string, SerializableClassInfoPtr> serializables;
};

class SerializeFunctionInfoMediator {
public:
    friend class SerializationContext;

    static bool AddSerializeDecl(FunctionDecl* decl, SerializeFunctionInfoPtr&& serializable);

    static void Reset();
private:
    static std::unordered_map<std::string, SerializeFunctionInfoPtr> serializables;
};

class ClassAnalyzer {
public:
    //static bool hasSerializeMethod(const CXXRecordDecl* serializable);
    static bool FetchSerializeMethod(const CXXRecordDecl* serializable, /* out */ FunctionTemplateDecl*& serializeDecl);
    static void FetchSerializableMembers(const CXXRecordDecl* serializable, SerializableClassInfoPtr classInfo);
    //static std::vector<std::string> getSerializableMembers(const FunctionTemplateDecl* serialize_function);
    static void AnalyzeSerializeMethod(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo);
};