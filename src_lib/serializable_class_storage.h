#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

class SerializableClassInfo {
    std::string m_name;
    std::vector<std::string> m_serializableFields;
};

class SerializableCXXRecordDeclStorage {
public:
    static bool AddSerializableDecl(CXXRecordDecl* serializable) {
        serializables.push_back(serializable);
        return true;
    }

    static const std::vector<CXXRecordDecl*>& GetClasses() {
        return serializables;
    }
private:
    static std::vector<CXXRecordDecl*> serializables;
};

class SerializableInClassAnalyzer {
public:
    static bool hasSerializeMethod(const CXXRecordDecl* serializable) {
        // TODO: Find non-intrusive definition serialize
        // TODO: Check if split_member

        auto it = std::find_if(serializable->method_begin(), serializable->method_end(), [&](const auto* method){
            return method->getNameAsString() == "serialize";
        });
        if (it == serializable->method_end()) {
            return false;
        }
        return true;
    }
}