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

// TODO: Isn't this finding intrusive?
void FindIntrusiveMethod(FunctionTemplateDecl*& serialize, const CXXRecordDecl* serializable) {
    auto decls = serializable->decls();

    auto serialize_it = std::find_if(decls.begin(), decls.end(), [](const Decl* decl){
        if (auto templateMethod = dyn_cast<FunctionTemplateDecl>(decl)) {
            if (templateMethod->getNameAsString() == "serialize") {
                return true;
            }
        }
        return false;
    });

    serialize = (serialize_it != decls.end()) ? dyn_cast<FunctionTemplateDecl>(*serialize_it) : nullptr;
}

void FindIntrusiveSplitMethod(FunctionTemplateDecl*& save, FunctionTemplateDecl*& load, const CXXRecordDecl* serializable) {
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

ISerializeFunctionInfoPtr ParseSplitSerializationFunctionBodies(clang::ASTContext& context, const CXXRecordDecl* serializableClass) {
    FunctionTemplateDecl* save;
    FunctionTemplateDecl* load;
    FindIntrusiveSplitMethod(save, load, serializableClass);
    // TODO: Right now, we can only be here if (and only if) a save and load exist intrusively. Need to handle non-intrusive split by looking at the macros.

    SerializeFunctionInfoPtr save_info = ParseSerializationFunctionBody(context, save);
    SerializeFunctionInfoPtr load_info = ParseSerializationFunctionBody(context, load);

    std::string saveFilename, loadFileName;
    unsigned int saveLine, loadLine, saveColumn, loadColumn;
    Utils::GetFullLocaionOfDecl(context, save, saveFilename, saveLine, saveColumn);
    Utils::GetFullLocaionOfDecl(context, load, loadFileName, loadLine, loadColumn);

    auto splitSerializeMethodInfo = std::make_shared<SplitFunctionInfo_Intrusive>(std::move(save_info), std::move(load_info));

    return splitSerializeMethodInfo;
}

/// @brief Fetches serialize method, or the split save/load methods, from INSIDE the class declaration (intrusive), if one exists (another attempt is made during mediation)
/// Phase: Discovery.
/// @param serializable - AST node for the serializable class.
/// @param serializeDecl - Out-parameter, the intrusive serialize-method
/// @return true if intrusive methods were found and processed, false if no methods were found.
/// @exception SerializationParseException if methods were found but could not be parsed.
bool DiscoveryHelper::FetchSerializeMethod(clang::ASTContext& context, const CXXRecordDecl* serializableClass, SerializableClassInfoPtr classInfo) {
    // Serialize methods are always template functions

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

    FunctionTemplateDecl* serialize;
    FindIntrusiveMethod(serialize, serializableClass);

    if (serialize == nullptr) {
        // No serialize function found. It may therefore be:
        // - Non-intrusively serialized.
        // - Missing (which should be caught by the compiler?)
        classInfo->SetInfo(SerializationInfoFlag::Info_NonIntrusive);
        return false;
    }

    // May be that the serialize-function exists but it is macro-expanded
    if (SplitMemberMacro::ClassContainsSplitMacro(context, serializableClass)) {
        // This is the case, so immediately find and parse the save/load methods.
        ISerializeFunctionInfoPtr splitMethodInfo = ParseSplitSerializationFunctionBodies(context, serializableClass);
        classInfo->SetSerializeMethodInfo(splitMethodInfo);
        classInfo->SetInfo(SerializationInfoFlag::Info_UsesSplitMacro);
        classInfo->SetInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization);
        return true;
    }
    
    // No macro found. Parse the serialize function
    SerializeFunctionInfoPtr methodInfo = ParseSerializationFunctionBody(context, serialize);

    // Check if we have detected the ::split_member call.
    if (methodInfo->SplitsInternally()) {
        // If so, parse the find and parse the save/load methods.
        ISerializeFunctionInfoPtr splitmethodInfo = ParseSplitSerializationFunctionBodies(context, serializableClass);
        classInfo->SetSerializeMethodInfo(splitmethodInfo);
        classInfo->SetInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization);
        return true;
    }
    // If we are here, the function is actually just normal. We found the non-intrusive, the class had
    // no macro, and it did not use the ::split_member.

    classInfo->SetSerializeMethodInfo(methodInfo);

    return true;
}