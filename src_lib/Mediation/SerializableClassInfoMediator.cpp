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

void FindIntrusiveSplit(FunctionTemplateDecl*& save, FunctionTemplateDecl*& load, const CXXRecordDecl* serializable) {
    auto decls = serializable->decls();

    auto save_it = std::find_if(decls.begin(), decls.end(), [](const Decl* decl){
        if (auto templateMethod = dyn_cast<FunctionTemplateDecl>(decl)) {
            if (templateMethod->getNameAsString() == "save") {
                return true;
            }
        }
        return false;
    });

    auto load_it = std::find_if(decls.begin(), decls.end(), [](const Decl* decl){
        if (auto templateMethod = dyn_cast<FunctionTemplateDecl>(decl)) {
            if (templateMethod->getNameAsString() == "load") {
                return true;
            }
        }
        return false;
    });

    save = (save_it != decls.end()) ? dyn_cast<FunctionTemplateDecl>(*save_it) : nullptr;
    load = (load_it != decls.end()) ? dyn_cast<FunctionTemplateDecl>(*load_it) : nullptr;
}

SerializeFunctionInfoPtr ParseSerializationFunctionBody(clang::ASTContext& context, FunctionTemplateDecl* serialize) {
    auto serializeMethod = serialize->getAsFunction();
    // Intrusive method found. Set it.
    auto intrusiveSerializeMethodInfo = InfoFactory::Create<SerializeFunctionInfo_Intrusive>(context, serializeMethod);
    auto body = serializeMethod->getBody();

    SerializableStmtVisitor visitor{&context};
    visitor.TraverseStmt(body);
    auto methodContents = visitor.GetOperations();

    for (auto& field : methodContents) {
        intrusiveSerializeMethodInfo->AddSerializableField(std::move(field));
    };
    intrusiveSerializeMethodInfo->SetSplitsInternally(visitor.IsSplitInternal());

    return intrusiveSerializeMethodInfo;
}

/// @brief Fetches serialize method, or the split save/load methods, from INSIDE the class declaration (intrusive), if one exists (another attempt is made during mediation)
/// Phase: Discovery.
/// @param serializable - AST node for the serializable class.
/// @param serializeDecl - Out-parameter, the intrusive serialize-method
/// @return true if intrusive methods were found and processed, false if no methods were found.
/// @exception SerializationParseException if methods were found but could not be parsed.
bool DiscoveryHelper::FetchSerializeMethod(clang::ASTContext& context, const CXXRecordDecl* serializableClass, SerializableClassInfoPtr classInfo) {
    // Serialize methods are always template functions

    auto decls = serializableClass->decls();

    // First, search the declarations for an intrusive serialize method.
    // If one is found, check it for the ::split_member declaration.
    //  - If found, serialization is split and we must then seek for an intrusive split.
    //  - If so, then find save/load, it is intrusively save/loaded and we handle accordingly.
    //  - If not, it is non-intrusively split and we cease seeking.
    // If the ::split_member is not found, we parse the serialize-method as normal.
    // If no serialize is found, we assume for now that it is serialized non-intrusively
    // Also must search for macro

    // Therefore, we begin with a search for the serialize-method:

    // fun SeekSerialize
    // If not found, assume not intrusively and return
    // Check if contains the ::split_member.
    // If not, parse as normal !IsSerializationSplit
    // Else, seek save load functions.
    // If found, parse as normal.
    // If not found, assume non-intrusively split and move on.

    if (!IsSerializationSplit(context, serializableClass)) {
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
            // No intrusive serialize method found. Setting the Decl to nullptr and returning false
            return false;
        }

        auto serializeDecl = dyn_cast<FunctionTemplateDecl>(*serialize_it);

        auto methodInfo = ParseSerializationFunctionBody(context, serializeDecl);

        // Check if the processing of the serialize-method discovered the call to split_member.
        // If so, do not set the serialize-method as it is assumed empty.
        if (!methodInfo->SplitsInternally()) {
            classInfo->SetSerializeMethodInfo(methodInfo);
            return true;
        }
    }

    if (!IsSerializationSplit(context, serializableClass)) {
        return false;
    }

    // Missed return for non-split. So it must be split, or not exist.
    FunctionTemplateDecl* save;
    FunctionTemplateDecl* load;
    FindIntrusiveSplit(save, load, serializableClass);
    // TODO: Right now, we can only be here if (and only if) a save and load exist intrusively. Need to handle non-intrusive split by looking at the macros.

    SerializeFunctionInfoPtr save_info = ParseSerializationFunctionBody(context, save);
    SerializeFunctionInfoPtr load_info = ParseSerializationFunctionBody(context, load);

    std::string saveFilename, loadFileName;
    unsigned int saveLine, loadLine, saveColumn, loadColumn;
    Utils::GetFullLocaionOfDecl(context, save, saveFilename, saveLine, saveColumn);
    {
        auto body = save->getBody();

        SerializableStmtVisitor visitor{&context};
        visitor.TraverseStmt(body);
        auto methodContents = visitor.GetOperations();

        for (auto& field : methodContents) {
            save_info->AddSerializableField(std::move(field));
        };
    }
    
    Utils::GetFullLocaionOfDecl(context, load, loadFileName, loadLine, loadColumn);
    {
        auto body = load->getBody();

        SerializableStmtVisitor visitor{&context};
        visitor.TraverseStmt(body);
        auto methodContents = visitor.GetOperations();

        for (auto& field : methodContents) {
            load_info->AddSerializableField(std::move(field));
        };
    }

    auto splitSerializeMethodInfo = std::make_shared<SplitFunctionInfo_Intrusive>(std::move(save_info), std::move(load_info));

    classInfo->SetSerializeMethodInfo(splitSerializeMethodInfo);

    return true;
}

// TODO: The save/load methods are declared by BOOST_SERIALIZATION_SPLIT_MEMBER macro.
// The current code is a placeholder workaround to find it.
bool DiscoveryHelper::IsSerializationSplit(ASTContext& context, const CXXRecordDecl* serializable) {

    FunctionTemplateDecl* save;
    FunctionTemplateDecl* load;

    FindIntrusiveSplit(save, load, serializable);

    if (save == nullptr || load == nullptr) {
        return false;
    }
    
    return true;
}