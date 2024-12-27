#pragma once

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

class SH_Tests : public ::testing::Test {
protected:
    void SetUp() override {
    SerializableClassInfoMediator::Reset();
    }

    void TearDown() override {
        SerializableClassInfoMediator::Reset();
    }
};


TEST_F(SH_Tests, BasicAnalysis) {
    std::string file = "./projects/basic/basic_class.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    ASSERT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableClassInfoMediator::FlattenSerializableContainer();

    ASSERT_EQ(serializable_classes.size(), 6);

    auto class_Basic_NotSerialized = GetClassFromVector(serializable_classes, "Basic_NotSerialized");
    auto class_Basic_SerializableWithoutFunction = GetClassFromVector(serializable_classes, "Basic_SerializableWithoutFunction");
    auto class_Basic_AllMembersSerialized = GetClassFromVector(serializable_classes, "Basic_AllMembersSerialized");
    auto class_Basic_OneMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_OneMemberNotSerialized");
    auto class_Basic_TaggedMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_TaggedMemberNotSerialized");
    auto class_Basic_UntaggedMemberSeralized = GetClassFromVector(serializable_classes, "Basic_UntaggedMemberSeralized");
    auto class_Basic_TwoErrorsAtOnce = GetClassFromVector(serializable_classes, "Basic_TwoErrorsAtOnce");

    ASSERT_TRUE(class_Basic_NotSerialized.expired());
    ASSERT_FALSE(class_Basic_SerializableWithoutFunction.expired());
    ASSERT_FALSE(class_Basic_AllMembersSerialized.expired());
    ASSERT_FALSE(class_Basic_OneMemberNotSerialized.expired());
    ASSERT_FALSE(class_Basic_TaggedMemberNotSerialized.expired());
    ASSERT_FALSE(class_Basic_UntaggedMemberSeralized.expired());
    ASSERT_FALSE(class_Basic_TwoErrorsAtOnce.expired());
    auto class_Basic_SerializableWithoutFunction_Ptr = class_Basic_SerializableWithoutFunction.lock();
    auto class_Basic_AllMembersSerialized_Ptr = class_Basic_AllMembersSerialized.lock();
    auto class_Basic_OneMemberNotSerialized_Ptr = class_Basic_OneMemberNotSerialized.lock();
    auto class_Basic_TaggedMemberNotSerialized_Ptr = class_Basic_TaggedMemberNotSerialized.lock();
    auto class_Basic_UntaggedMemberSeralized_Ptr = class_Basic_UntaggedMemberSeralized.lock();
    auto class_Basic_TwoErrorsAtOnce_Ptr = class_Basic_TwoErrorsAtOnce.lock();
    
    ASSERT_TRUE(class_Basic_SerializableWithoutFunction_Ptr->HasError(SerializationError::Error_SerializeMethodNotFound));
    ASSERT_TRUE(class_Basic_AllMembersSerialized_Ptr->HasError(SerializationError::Error_NoError));
    ASSERT_TRUE(class_Basic_OneMemberNotSerialized_Ptr->HasError(SerializationError::Error_NoError));
    ASSERT_TRUE(class_Basic_TaggedMemberNotSerialized_Ptr->HasError(SerializationError::Error_MarkedFieldNotSerialized));
    ASSERT_TRUE(class_Basic_UntaggedMemberSeralized_Ptr->HasError(SerializationError::Error_UnmarkedFieldSerialized));
    ASSERT_TRUE(class_Basic_TwoErrorsAtOnce_Ptr->HasError(SerializationError::Error_UnmarkedFieldSerialized));
    ASSERT_TRUE(class_Basic_TwoErrorsAtOnce_Ptr->HasError(SerializationError::Error_MarkedFieldNotSerialized));
}

TEST_F(SH_Tests, NonIntrusiveAnalysis) {
    std::string file = "./projects/non_intrusive/non_intrusive.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    ASSERT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableClassInfoMediator::FlattenSerializableContainer();

    ASSERT_EQ(serializable_classes.size(), 1);

    auto class_NonIntrusive_NoError = GetClassFromVector(serializable_classes, "NonIntrusive_NoError");

    ASSERT_FALSE(class_NonIntrusive_NoError.expired());

    auto class_NonIntrusive_NoError_Ptr = class_NonIntrusive_NoError.lock();

    ASSERT_TRUE(class_NonIntrusive_NoError_Ptr->HasError(SerializationError::Error_NoError));
}

