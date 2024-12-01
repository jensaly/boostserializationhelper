#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class SerializableClassInfo {
    std::string m_name;
    std::vector<std::string> m_serializableFields;
};

class SerializableCXXRecordDeclStorage {
public:
    static bool AddSerializableDecl(CXXRecordDecl* serializable);

    static const std::vector<CXXRecordDecl*>& GetClasses();
private:
    static std::vector<CXXRecordDecl*> serializables;
};

class SerializableInClassAnalyzer {
public:
    static bool hasSerializeMethod(const CXXRecordDecl* serializable);
    static FunctionTemplateDecl* getSerializeMethod(const CXXRecordDecl* serializable);
    static bool checkAllSerializeableInSerialize(const FunctionTemplateDecl* serialize_function, const CXXRecordDecl* serializable);
    static std::vector<std::string> getSerializableMembers(const CXXRecordDecl* serializable);
    static std::vector<std::string> getSerializableMembers(const FunctionTemplateDecl* serialize_function);
};