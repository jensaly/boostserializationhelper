#include <PostProcessing/SerializableClassInfo.h>
#include <PostProcessing/SerializableFieldInfo.h>
#include <Internals/Internals.h>
#include <iostream>

std::unordered_map<std::string, SerializableClassInfoPtr> SerializableClassInfoMediator::serializables{};

bool SerializableClassInfoMediator::AddSerializableDecl(CXXRecordDecl* decl, SerializableClassInfoPtr&& serializable) {
    if (serializables.find(decl->getQualifiedNameAsString()) != serializables.end()) {
        return false;
    }
    serializables.insert({decl->getQualifiedNameAsString(), std::move(serializable)});
    return true;
}

std::vector<SerializableClassInfoWeakPtr> SerializableClassInfoMediator::FlattenSerializableContainer() {
    std::vector<SerializableClassInfoWeakPtr> classes{};
    for (auto& [decl, classPtr] : serializables) {
        auto ptr = SerializableClassInfoWeakPtr(classPtr);
        classes.push_back(ptr);
    }
    return classes;
}

void SerializableClassInfoMediator::Reset() {
    serializables.clear();
}

std::unordered_map<std::string, SerializeFunctionInfoPtr> SerializeFunctionInfoMediator::serializables{};

bool SerializeFunctionInfoMediator::AddSerializeDecl(FunctionTemplateDecl* decl, SerializeFunctionInfoPtr&& serializable) {
    if (serializables.find(decl->getQualifiedNameAsString()) != serializables.end()) {
        return false;
    }
    serializables.insert({decl->getQualifiedNameAsString(), std::move(serializable)});
    return true;
}

void SerializeFunctionInfoMediator::Reset() {
    serializables.clear();
}

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

/// @brief Fetches serialize method from INSIDE the class declaration (intrusive), if one exists (another attempt is made during mediation)
/// Phase: Discovery.
/// @param serializable - AST node for the serializable class.
/// @param serializeDecl - Out-parameter, the intrusive serialize-method
/// @return true if an intrusive method was found, false otherwise
bool ClassAnalyzer::FetchSerializeMethod(const CXXRecordDecl* serializable, /*out*/ FunctionTemplateDecl*& serializeDecl) {
    // Serialize methods are always template functions

    auto decls = serializable->decls();

    // Search the decl to see if the method is found internally.
    auto serialize_it = std::find_if(decls.begin(), decls.end(), [](const Decl* decl){
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
    
    serializeDecl = nullptr;
    return false;
}

void ClassAnalyzer::AnalyzeSerializeMethod(clang::FunctionTemplateDecl* serializeMethod, SerializableClassInfoPtr classInfo) {
    classInfo->SetError(checkAllSerializeableInSerialize(serializeMethod, classInfo));
}

/*
std::vector<std::string> ClassAnalyzer::getSerializableMembers(const FunctionTemplateDecl* serialize_function) {

}
*/

SerializationError ClassAnalyzer::checkAllSerializeableInSerialize(const FunctionTemplateDecl* serialize_function, const SerializableClassInfoPtr serializableClass) {
    SerializationError error = SerializationError::Error_NoError;

    const auto* function_decl = serialize_function->getTemplatedDecl();
    auto body = function_decl->getBody();

    SerializableStmtVisitor visitor{};
    visitor.TraverseStmt(body);
    auto methodContents = visitor.GetSerializeContents();
    auto classFields = serializableClass->GetFields();

    for (auto& field : classFields) {
        if (std::find(methodContents.begin(), methodContents.end(), field.GetName()) == methodContents.end()) {
            error |= SerializationError::Error_MarkedFieldNotSerialized;
        }
    }

    for (auto& field : methodContents) {
        if (std::find_if(classFields.begin(), classFields.end(), [field](SerializableFieldInfo& info){ return info.GetName() == field;}) == classFields.end()) {
            error |= SerializationError::Error_UnmarkedFieldSerialized;
        }
    }
    return error;
}
