#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class SerializableClassInfo;

using SerializableClassName = std::string;
using SerializableClassInfoPtr = std::shared_ptr<SerializableClassInfo>;
using SerializableClassInfoWeakPtr = std::weak_ptr<const SerializableClassInfo>;

class SerializableClassInfoAggregator {
public:
    static bool AddSerializableDecl(SerializableClassInfoPtr&& serializable);

    static std::vector<SerializableClassInfoWeakPtr> FlattenSerializableContainer();
private:
    static std::unordered_map<SerializableClassName, SerializableClassInfoPtr> serializables;
};

class ClassAnalyzer {
public:
    //static bool hasSerializeMethod(const CXXRecordDecl* serializable);
    static bool FetchSerializeMethod(const CXXRecordDecl* serializable, /* out */ FunctionTemplateDecl* serializeDecl);
    //static bool checkAllSerializeableInSerialize(const FunctionTemplateDecl* serialize_function, const CXXRecordDecl* serializable);
    static void FetchSerializableMembers(const CXXRecordDecl* serializable, SerializableClassInfoPtr classInfo);
    //static std::vector<std::string> getSerializableMembers(const FunctionTemplateDecl* serialize_function);
};