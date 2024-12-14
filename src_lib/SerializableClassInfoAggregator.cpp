#include <PostProcessing/SerializableClassInfo.h>
#include <Internals/Internals.h>
#include <iostream>

std::unordered_map<SerializableClassName, SerializableClassInfoPtr> SerializableClassInfoAggregator::serializables{};

bool SerializableClassInfoAggregator::AddSerializableDecl(SerializableClassInfoPtr&& serializable) {
    auto name = serializable->GetClassName();
    if (serializables.find(name) != serializables.end()) {
        return false;
    }
    serializables.insert({name, std::move(serializable)});
    return true;
}

std::vector<SerializableClassInfoWeakPtr> SerializableClassInfoAggregator::FlattenSerializableContainer() {
    std::vector<SerializableClassInfoWeakPtr> classes{};
    for (auto& [name, classPtr] : serializables) {
        auto ptr = SerializableClassInfoWeakPtr(classPtr);
        classes.push_back(ptr);
    }
    return classes;
}
/*
bool ClassAnalyzer::hasSerializeMethod(const CXXRecordDecl* serializable) {
    return getSerializeMethod(serializable) != nullptr;
}

FunctionTemplateDecl* ClassAnalyzer::getSerializeMethod(const CXXRecordDecl* serializable) {
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
*/
void ClassAnalyzer::FetchSerializableMembers(const CXXRecordDecl* serializable, SerializableClassInfoPtr classInfo) {
    std::vector<std::string> serializable_members;
    if (!serializable->hasDefinition()) {
        return;
    }

    for (auto field : serializable->fields()) {
        if (!field->hasAttr<AnnotateAttr>()) {
            continue;
        }
        auto attr = serializable->getAttr<AnnotateAttr>();
        if (attr->getAnnotation() != "serializable") {
            continue;
        }
        SerializableFieldInfo fieldInfo{field->getNameAsString()};
        classInfo->AddSerializableField(fieldInfo);
    }

    return;
}

bool ClassAnalyzer::FetchSerializeMethod(const CXXRecordDecl* serializable, FunctionTemplateDecl* serializeDecl) {
    auto decls = serializable->decls();

    auto serialize_it = std::find_if(decls.begin(), decls.end(), [](const Decl* decl){
        // Inline serialize-method is always a template function
        if (auto templateMethod = dyn_cast<FunctionTemplateDecl>(decl)) {
            if (templateMethod->getNameAsString() == "serialize") {
                
                return true;
            }
        }
        return false;
    });
    
    if (serialize_it != decls.end()) {

        serializeDecl = dyn_cast<FunctionTemplateDecl>(*serialize_it);
        return true;
    }
    
    return false;
}
/*
std::vector<std::string> ClassAnalyzer::getSerializableMembers(const FunctionTemplateDecl* serialize_function) {

}
*/

/*
bool ClassAnalyzer::checkAllSerializeableInSerialize(const FunctionTemplateDecl* serialize_function, const CXXRecordDecl* serializable) {
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
*/