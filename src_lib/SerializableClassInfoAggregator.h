#pragma once

#include "clang/AST/AST.h"
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

class SerializableClassInfoAggregator {
public:
    static bool AddSerializableDecl(SerializableClassInfoPtr&& serializable);

    static std::vector<SerializableClassInfoWeakPtr> FlattenSerializableContainer();

    static void Reset();
private:
    static std::unordered_map<CXXRecordDecl*, SerializableClassInfoPtr> serializables;
};

class SerializeFunctionInfoAggregator {
public:
    static bool AddSerializeDecl(SerializeFunctionInfoPtr&& serializable);

    static void Reset();
private:
    static std::unordered_map<FunctionTemplateDecl*, SerializeFunctionInfoPtr> serializables;
};

class ClassAnalyzer {
public:
    //static bool hasSerializeMethod(const CXXRecordDecl* serializable);
    static bool FetchSerializeMethod(const CXXRecordDecl* serializable, /* out */ FunctionTemplateDecl*& serializeDecl);
    static SerializationError checkAllSerializeableInSerialize(const FunctionTemplateDecl* serialize_function, const SerializableClassInfoPtr serializable);
    static void FetchSerializableMembers(const CXXRecordDecl* serializable, SerializableClassInfoPtr classInfo);
    //static std::vector<std::string> getSerializableMembers(const FunctionTemplateDecl* serialize_function);
    static void AnalyzeSerializeMethod(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo);
};