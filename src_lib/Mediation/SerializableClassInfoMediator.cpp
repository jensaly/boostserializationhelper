// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <Internals/Internals.h>
#include <SerializationInfo/InfoFactory.h>
#include <SerializationInfo/SerializableClassInfo.h>
#include <SerializationInfo/SerializableFieldInfo.h>
#include <Utils/Utils.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

using namespace clang;

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

bool SerializeFunctionInfoMediator::AddSerializeDecl(clang::FunctionDecl* decl, SerializeFunctionInfoPtr&& serializable) {
    // Preceeding function has ascertained that there is at least 2 arguments
    auto ParamType = decl->getParamDecl(1)->getType();
    const Type *UnderlyingType = ParamType.getTypePtr()->getUnqualifiedDesugaredType();
    std::string ClassName = "";
    
    // Underlying type for non-intrusive serialize-functions is a pointer or reference.
    if (UnderlyingType->isPointerType() || UnderlyingType->isReferenceType()) {
        QualType PointeeType = UnderlyingType->getPointeeType();
        if (const auto *Record = PointeeType->getAs<RecordType>()) {
            ClassName = Record->getDecl()->getNameAsString();
        }
        else {
            return false;
        }
    }

    // Already registered. Should never happen.
    if (serializables.find(ClassName) != serializables.end()) {
        return false;
    }
    serializables.insert({ClassName, std::move(serializable)});
    return true;
}

void SerializeFunctionInfoMediator::Reset() {
    serializables.clear();
}

/// @brief Fetches serializable members of the RecordDecl
/// Phase: Discovery
/// @param serializable - The CXXRecordDecl for the serializable class
/// @param classInfo - The serializable class info
void DiscoveryHelper::FetchSerializableMembers(clang::ASTContext& context, const CXXRecordDecl* serializable, SerializableClassInfoPtr classInfo) {
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

        auto fieldInfo = InfoFactory::Create<SerializableFieldInfo>(context, field);
        classInfo->AddSerializableField(std::move(fieldInfo));
    }

    return;
}

/// @brief Fetches serialize method from INSIDE the class declaration (intrusive), if one exists (another attempt is made during mediation)
/// Phase: Discovery.
/// @param serializable - AST node for the serializable class.
/// @param serializeDecl - Out-parameter, the intrusive serialize-method
/// @return true if an intrusive method was found, false otherwise
bool DiscoveryHelper::FetchSerializeMethod(clang::ASTContext& context, const CXXRecordDecl* serializable, /*out*/ FunctionTemplateDecl*& serializeDecl) {
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
    
    if (serialize_it == decls.end()) {
        // No intrusive method found. Setting the Decl to nullptr and returning false
        serializeDecl = nullptr;
        return false;
    }
    
    serializeDecl = dyn_cast<FunctionTemplateDecl>(*serialize_it);
    return true;
}