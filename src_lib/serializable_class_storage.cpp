#include "serializable_class.h"
#include <iostream>

std::vector<CXXRecordDecl*> SerializableCXXRecordDeclStorage::serializables{};

bool SerializableCXXRecordDeclStorage::AddSerializableDecl(CXXRecordDecl* serializable) {
    serializables.push_back(serializable);
    return true;
}

const std::vector<CXXRecordDecl*>& SerializableCXXRecordDeclStorage::GetClasses() {
    return serializables;
}

bool SerializableInClassAnalyzer::hasSerializeMethod(const CXXRecordDecl* serializable) {
    return getSerializeMethod(serializable) != nullptr;
}

FunctionTemplateDecl* SerializableInClassAnalyzer::getSerializeMethod(const CXXRecordDecl* serializable) {
    auto decls = serializable->decls();

    auto serialize_it = std::find_if(decls.begin(), decls.end(), [](const Decl* decl){
        if (auto template_method = dyn_cast<FunctionTemplateDecl>(decl)) {
            if (template_method->getNameAsString() == "serialize") {
                return true;
            }
        }
        return false;
    });
    
    if (serialize_it != decls.end()) {
        return dyn_cast<FunctionTemplateDecl>(*serialize_it);
    }
    
    return nullptr;
}

std::vector<std::string> SerializableInClassAnalyzer::getSerializableMembers(const CXXRecordDecl* serializable) {
    std::vector<std::string> serializable_members;

    if (!serializable->hasDefinition()) {
        return serializable_members;
    }

    llvm::outs() << "Class/Struct: " << serializable->getNameAsString();

    if (serializable->hasAttr<AnnotateAttr>()) {
        const AnnotateAttr *attr = serializable->getAttr<AnnotateAttr>();
        llvm::outs() << " [Annotation: " << attr->getAnnotation() << "]";
    } else {
        llvm::outs() << " [No annotation]";
    }

    llvm::outs() << "\n";

    for (auto field : serializable->fields()) {
        if (field->hasAttr<AnnotateAttr>()) {
        llvm::outs() <<field->getAttr<AnnotateAttr>()->getAnnotation() << "\n";
        }
        llvm::outs() << field->getNameAsString() << "\n";
    }

    return serializable_members;
}

std::vector<std::string> SerializableInClassAnalyzer::getSerializableMembers(const FunctionTemplateDecl* serialize_function) {

}

bool SerializableInClassAnalyzer::checkAllSerializeableInSerialize(const FunctionTemplateDecl* serialize_function, const CXXRecordDecl* serializable) {
    const auto* function_decl = serialize_function->getTemplatedDecl();
    auto body = function_decl->getBody();

    SerializableStmtVisitor visitor{};
    visitor.TraverseStmt(body);
    auto serializeContents = visitor.GetSerializeContents();
    auto fields = getSerializableMembers(serializable);

    for (auto& field : fields) {
        if (std::find(serializeContents.begin(), serializeContents.end(), field) == serializeContents.end()) {
            return false;
        }
    }
    return true;
}