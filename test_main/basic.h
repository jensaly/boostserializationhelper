#include <gtest/gtest.h>
#include <basic/basic_class.hpp>
#include "Internals/Internals.h"
#include <fstream>
#include <sstream>
#include "PostProcessing/SerializableClassInfo.h"

using SerializableClassInfoWeakPtr = std::weak_ptr<const SerializableClassInfo>;

std::string GetCodeFromSourceFile(const std::string& filepath) {
    std::ifstream file;
    file.open(filepath);

    if (!file.is_open()) {
        std::cout << "Could not open " + filepath;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

SerializableClassInfoWeakPtr GetClassFromVector(std::vector<SerializableClassInfoWeakPtr> decls, std::string name) {
    auto it = std::find_if(decls.begin(), decls.end(), [name](const SerializableClassInfoWeakPtr decl){
        if (decl.expired()) {
            throw std::exception();
        }
        auto ptr = decl.lock();
        return ptr->GetClassName() == name;
    });
    return (it != decls.end()) ? *it : SerializableClassInfoWeakPtr();
}

TEST(SH_BasicProject_Tests, BasicAnalysis) {
    std::string file = "./projects/basic/basic_class.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    ASSERT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableClassInfoAggregator::FlattenSerializableContainer();

    ASSERT_EQ(serializable_classes.size(), 5);

    auto class_Basic_NotSerialized = GetClassFromVector(serializable_classes, "Basic_NotSerialized");
    auto class_Basic_SerializableWithoutFunction = GetClassFromVector(serializable_classes, "Basic_SerializableWithoutFunction");
    auto class_Basic_AllMembersSerialized = GetClassFromVector(serializable_classes, "Basic_AllMembersSerialized");
    auto class_Basic_OneMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_OneMemberNotSerialized");
    auto class_Basic_TaggedMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_TaggedMemberNotSerialized");
    auto class_Basic_UntaggedMemberSeralized = GetClassFromVector(serializable_classes, "Basic_UntaggedMemberSeralized");

    ASSERT_TRUE(class_Basic_NotSerialized.expired());
    ASSERT_FALSE(class_Basic_SerializableWithoutFunction.expired());
    ASSERT_FALSE(class_Basic_AllMembersSerialized.expired());
    ASSERT_FALSE(class_Basic_OneMemberNotSerialized.expired());
    ASSERT_FALSE(class_Basic_TaggedMemberNotSerialized.expired());
    ASSERT_FALSE(class_Basic_UntaggedMemberSeralized.expired());
    auto class_Basic_SerializableWithoutFunction_Ptr = class_Basic_SerializableWithoutFunction.lock();
    auto class_Basic_AllMembersSerialized_Ptr = class_Basic_AllMembersSerialized.lock();
    auto class_Basic_OneMemberNotSerialized_Ptr = class_Basic_OneMemberNotSerialized.lock();
    auto class_Basic_TaggedMemberNotSerialized_Ptr = class_Basic_TaggedMemberNotSerialized.lock();
    auto class_Basic_UntaggedMemberSeralized_Ptr = class_Basic_UntaggedMemberSeralized.lock();
    
    ASSERT_TRUE(class_Basic_SerializableWithoutFunction_Ptr->HasError(SerializationError::Error_SerializeMethodNotFound));
    ASSERT_TRUE(class_Basic_AllMembersSerialized_Ptr->HasError(SerializationError::Error_NoError));
    ASSERT_TRUE(class_Basic_OneMemberNotSerialized_Ptr->HasError(SerializationError::Error_NoError));
    ASSERT_TRUE(class_Basic_TaggedMemberNotSerialized_Ptr->HasError(SerializationError::Error_MarkedFieldNotSerialized));
    ASSERT_TRUE(class_Basic_UntaggedMemberSeralized_Ptr->HasError(SerializationError::Error_UnmarkedFieldSerialized));
    /*
    auto method_Basic_AllMembersSerialized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_AllMembersSerialized);
    auto method_Basic_OneMemberNotSerialized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_OneMemberNotSerialized);
    auto method_Basic_TaggedMemberNotSerialized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_TaggedMemberNotSerialized);
    auto method_Basic_UntaggedMemberSeralized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_UntaggedMemberSeralized);

    ASSERT_TRUE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_AllMembersSerialized, class_Basic_AllMembersSerialized));
    ASSERT_TRUE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_OneMemberNotSerialized, class_Basic_OneMemberNotSerialized));
    ASSERT_FALSE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_TaggedMemberNotSerialized, class_Basic_TaggedMemberNotSerialized));
    ASSERT_TRUE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_UntaggedMemberSeralized, class_Basic_UntaggedMemberSeralized));
    */
}