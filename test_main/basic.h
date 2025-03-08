#pragma once

#include <gtest/gtest.h>
#include <basic/basic_class.hpp>
#include "Internals/Internals.h"
#include <fstream>
#include <sstream>
#include "SerializationInfo/SerializableClassInfo.h"
#include "SerializationContext.h"
#include <Diagnostics/TestDiagnosticReporter.h>

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
        SerializationContext::Reset();
    }
};


TEST_F(SH_Tests, BasicAnalysis) {
    SerializationContext::SetDiagnosticsReporter(std::make_unique<TestDiagnosticReporter>("BasicSingleTUTests.log"));
    std::string file = "./projects/basic/basic_class.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    EXPECT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableClassInfoMediator::FlattenSerializableContainer();

    EXPECT_EQ(serializable_classes.size(), 6);

    auto class_Basic_NotSerialized = GetClassFromVector(serializable_classes, "Basic_NotSerialized");
    auto class_Basic_SerializableWithoutFunction = GetClassFromVector(serializable_classes, "Basic_SerializableWithoutFunction");
    auto class_Basic_AllMembersSerialized = GetClassFromVector(serializable_classes, "Basic_AllMembersSerialized");
    auto class_Basic_OneMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_OneMemberNotSerialized");
    auto class_Basic_TaggedMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_TaggedMemberNotSerialized");
    auto class_Basic_UntaggedMemberSeralized = GetClassFromVector(serializable_classes, "Basic_UntaggedMemberSeralized");
    auto class_Basic_TwoErrorsAtOnce = GetClassFromVector(serializable_classes, "Basic_TwoErrorsAtOnce");

    EXPECT_TRUE(class_Basic_NotSerialized.expired());
    EXPECT_FALSE(class_Basic_SerializableWithoutFunction.expired());
    EXPECT_FALSE(class_Basic_AllMembersSerialized.expired());
    EXPECT_FALSE(class_Basic_OneMemberNotSerialized.expired());
    EXPECT_FALSE(class_Basic_TaggedMemberNotSerialized.expired());
    EXPECT_FALSE(class_Basic_UntaggedMemberSeralized.expired());
    EXPECT_FALSE(class_Basic_TwoErrorsAtOnce.expired());
    auto class_Basic_SerializableWithoutFunction_Ptr = class_Basic_SerializableWithoutFunction.lock();
    auto class_Basic_AllMembersSerialized_Ptr = class_Basic_AllMembersSerialized.lock();
    auto class_Basic_OneMemberNotSerialized_Ptr = class_Basic_OneMemberNotSerialized.lock();
    auto class_Basic_TaggedMemberNotSerialized_Ptr = class_Basic_TaggedMemberNotSerialized.lock();
    auto class_Basic_UntaggedMemberSeralized_Ptr = class_Basic_UntaggedMemberSeralized.lock();
    auto class_Basic_TwoErrorsAtOnce_Ptr = class_Basic_TwoErrorsAtOnce.lock();
    
    // Testing errors in detail to ensure it's not false positive
    EXPECT_TRUE(class_Basic_SerializableWithoutFunction_Ptr->HasError(SerializationErrorFlag::Error_SerializeMethodNotFound));
    EXPECT_FALSE(class_Basic_SerializableWithoutFunction_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Basic_SerializableWithoutFunction_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_FALSE(class_Basic_SerializableWithoutFunction_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Basic_SerializableWithoutFunction_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Basic_SerializableWithoutFunction_Ptr->HasInfo(SerializationInfoFlag::Info_NoInfo));
    EXPECT_FALSE(class_Basic_SerializableWithoutFunction_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_Basic_AllMembersSerialized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_FALSE(class_Basic_AllMembersSerialized_Ptr->HasError(SerializationErrorFlag::Error_SerializeMethodNotFound));
    EXPECT_FALSE(class_Basic_AllMembersSerialized_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Basic_AllMembersSerialized_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_EQ(class_Basic_AllMembersSerialized_Ptr->NumberOfErrors(), 0);
    EXPECT_TRUE(class_Basic_AllMembersSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_NoInfo));
    EXPECT_FALSE(class_Basic_AllMembersSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_Basic_OneMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_FALSE(class_Basic_OneMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_SerializeMethodNotFound));
    EXPECT_FALSE(class_Basic_OneMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Basic_OneMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_EQ(class_Basic_OneMemberNotSerialized_Ptr->NumberOfErrors(), 0);
    EXPECT_TRUE(class_Basic_OneMemberNotSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_NoInfo));
    EXPECT_FALSE(class_Basic_OneMemberNotSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_Basic_TaggedMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Basic_TaggedMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_SerializeMethodNotFound));
    EXPECT_FALSE(class_Basic_TaggedMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_FALSE(class_Basic_TaggedMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_EQ(class_Basic_TaggedMemberNotSerialized_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Basic_TaggedMemberNotSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_NoInfo));
    EXPECT_FALSE(class_Basic_TaggedMemberNotSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_Basic_UntaggedMemberSeralized_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_FALSE(class_Basic_UntaggedMemberSeralized_Ptr->HasError(SerializationErrorFlag::Error_SerializeMethodNotFound));
    EXPECT_FALSE(class_Basic_UntaggedMemberSeralized_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Basic_UntaggedMemberSeralized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Basic_UntaggedMemberSeralized_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Basic_UntaggedMemberSeralized_Ptr->HasInfo(SerializationInfoFlag::Info_NoInfo));
    EXPECT_FALSE(class_Basic_UntaggedMemberSeralized_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_Basic_TwoErrorsAtOnce_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_TRUE(class_Basic_TwoErrorsAtOnce_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Basic_TwoErrorsAtOnce_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_FALSE(class_Basic_TwoErrorsAtOnce_Ptr->HasError(SerializationErrorFlag::Error_SerializeMethodNotFound));
    EXPECT_EQ(class_Basic_TwoErrorsAtOnce_Ptr->NumberOfErrors(), 2);
    EXPECT_TRUE(class_Basic_TwoErrorsAtOnce_Ptr->HasInfo(SerializationInfoFlag::Info_NoInfo));
    EXPECT_FALSE(class_Basic_TwoErrorsAtOnce_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));
}

TEST_F(SH_Tests, NonIntrusiveAnalysis) {
    SerializationContext::SetDiagnosticsReporter(std::make_unique<TestDiagnosticReporter>("NonIntrusiveSingleTUTests.log"));
    std::string file = "./projects/non_intrusive/non_intrusive.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    EXPECT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableClassInfoMediator::FlattenSerializableContainer();

    EXPECT_EQ(serializable_classes.size(), 5);

    auto class_NonIntrusive_NoError = GetClassFromVector(serializable_classes, "NonIntrusive_NoError");
    auto class_NonIntrusive_TaggedMemberNotSerialized = GetClassFromVector(serializable_classes, "NonIntrusive_TaggedMemberNotSerialized");
    auto class_NonIntrusive_UntaggedMemberSeralized = GetClassFromVector(serializable_classes, "NonIntrusive_UntaggedMemberSeralized");
    auto class_NonIntrusive_SerializableWithoutFunction = GetClassFromVector(serializable_classes, "NonIntrusive_SerializableWithoutFunction");
    auto class_NonIntrusive_TwoErrorsAtOnce = GetClassFromVector(serializable_classes, "NonIntrusive_TwoErrorsAtOnce");

    EXPECT_FALSE(class_NonIntrusive_NoError.expired());
    EXPECT_FALSE(class_NonIntrusive_TaggedMemberNotSerialized.expired());
    EXPECT_FALSE(class_NonIntrusive_UntaggedMemberSeralized.expired());
    EXPECT_FALSE(class_NonIntrusive_SerializableWithoutFunction.expired());
    EXPECT_FALSE(class_NonIntrusive_TwoErrorsAtOnce.expired());

    auto class_NonIntrusive_NoError_Ptr = class_NonIntrusive_NoError.lock();
    auto class_NonIntrusive_TaggedMemberNotSerialized_Ptr = class_NonIntrusive_TaggedMemberNotSerialized.lock();
    auto class_NonIntrusive_UntaggedMemberSeralized_Ptr = class_NonIntrusive_UntaggedMemberSeralized.lock();
    auto class_NonIntrusive_SerializableWithoutFunction_Ptr = class_NonIntrusive_SerializableWithoutFunction.lock();
    auto class_NonIntrusive_TwoErrorsAtOnce_Ptr = class_NonIntrusive_TwoErrorsAtOnce.lock();

    EXPECT_TRUE(class_NonIntrusive_NoError_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_FALSE(class_NonIntrusive_NoError_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_NonIntrusive_NoError_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_EQ(class_NonIntrusive_NoError_Ptr->NumberOfErrors(), 0);
    EXPECT_TRUE(class_NonIntrusive_NoError_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_NonIntrusive_TaggedMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_NonIntrusive_TaggedMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_FALSE(class_NonIntrusive_TaggedMemberNotSerialized_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_EQ(class_NonIntrusive_TaggedMemberNotSerialized_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_NonIntrusive_TaggedMemberNotSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_NonIntrusive_UntaggedMemberSeralized_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_FALSE(class_NonIntrusive_UntaggedMemberSeralized_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_NonIntrusive_UntaggedMemberSeralized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_NonIntrusive_UntaggedMemberSeralized_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_NonIntrusive_UntaggedMemberSeralized_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));

    EXPECT_TRUE(class_NonIntrusive_SerializableWithoutFunction_Ptr->HasError(SerializationErrorFlag::Error_SerializeMethodNotFound));
    EXPECT_FALSE(class_NonIntrusive_SerializableWithoutFunction_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_NonIntrusive_SerializableWithoutFunction_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_NonIntrusive_SerializableWithoutFunction_Ptr->NumberOfErrors(), 1);
    EXPECT_FALSE(class_NonIntrusive_SerializableWithoutFunction_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive)); // Since it does not exist, it is by nature not intrusive.

    EXPECT_TRUE(class_NonIntrusive_TwoErrorsAtOnce_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_TRUE(class_NonIntrusive_TwoErrorsAtOnce_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_FALSE(class_NonIntrusive_TwoErrorsAtOnce_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_NonIntrusive_TwoErrorsAtOnce_Ptr->NumberOfErrors(), 2);
    EXPECT_TRUE(class_NonIntrusive_TwoErrorsAtOnce_Ptr->HasInfo(SerializationInfoFlag::Info_NonIntrusive));
}

TEST_F(SH_Tests, SplitAmpersandAnalysis) {
    SerializationContext::SetDiagnosticsReporter(std::make_unique<TestDiagnosticReporter>("SplitSingleTUTests.log"));
    std::string file = "./projects/split/split_ampersand.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    EXPECT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableClassInfoMediator::FlattenSerializableContainer();

    EXPECT_EQ(serializable_classes.size(), 10);

    auto class_Split_AllMembersSerialized = GetClassFromVector(serializable_classes, "Split_AllMembersSerialized_Ampersand");
    auto class_Split_AllMembersSerialized_NoMacro = GetClassFromVector(serializable_classes, "Split_AllMembersSerialized_Ampersand_NoMacro");
    auto class_Split_OneMemberNotSaved_NoMacro = GetClassFromVector(serializable_classes, "Split_OneMemberNotSaved_Ampersand_NoMacro");
    auto class_NonSplit_AllMembersSerialized_SplitMemberExists = GetClassFromVector(serializable_classes, "NonSplit_AllMembersSerialized_SplitMemberExists");
    auto class_Split_OneMemberNotSaved = GetClassFromVector(serializable_classes, "Split_OneMemberNotSaved_Ampersand");
    auto class_Split_OneMemberNotLoaded = GetClassFromVector(serializable_classes, "Split_OneMemberNotLoaded_Ampersand");
    auto class_Split_UnmarkedMemberSaved = GetClassFromVector(serializable_classes, "Split_UnmarkedMemberSavedAndLoaded_Ampersand");
    auto class_Split_MarkedMemberNotSavedOrLoaded = GetClassFromVector(serializable_classes, "Split_MarkedMemberNotSavedOrLoaded_Ampersand");
    auto class_Split_WrongOrder_TypeOk = GetClassFromVector(serializable_classes, "Split_WrongOrder_TypeOk_Ampersand");
    auto class_Split_WrongOrder_TypeWrong = GetClassFromVector(serializable_classes, "Split_WrongOrder_TypeWrong_Ampersand");

    EXPECT_FALSE(class_Split_AllMembersSerialized.expired());
    EXPECT_FALSE(class_Split_AllMembersSerialized_NoMacro.expired());
    EXPECT_FALSE(class_Split_OneMemberNotSaved_NoMacro.expired());
    EXPECT_FALSE(class_NonSplit_AllMembersSerialized_SplitMemberExists.expired());
    EXPECT_FALSE(class_Split_OneMemberNotSaved.expired());
    EXPECT_FALSE(class_Split_OneMemberNotLoaded.expired());
    EXPECT_FALSE(class_Split_UnmarkedMemberSaved.expired());
    EXPECT_FALSE(class_Split_MarkedMemberNotSavedOrLoaded.expired());
    EXPECT_FALSE(class_Split_WrongOrder_TypeOk.expired());
    EXPECT_FALSE(class_Split_WrongOrder_TypeWrong.expired());

    auto class_Split_AllMembersSerialized_Ptr = class_Split_AllMembersSerialized.lock();
    auto class_Split_AllMembersSerialized_NoMacro_Ptr = class_Split_AllMembersSerialized_NoMacro.lock();
    auto class_Split_OneMemberNotSaved_NoMacro_Ptr = class_Split_OneMemberNotSaved_NoMacro.lock();
    auto class_NonSplit_AllMembersSerialized_SplitMemberExists_Ptr = class_NonSplit_AllMembersSerialized_SplitMemberExists.lock();
    auto class_Split_OneMemberNotSaved_Ptr = class_Split_OneMemberNotSaved.lock();
    auto class_Split_OneMemberNotLoaded_Ptr = class_Split_OneMemberNotLoaded.lock();
    auto class_Split_UnmarkedMemberSavedAndLoaded_Ptr = class_Split_UnmarkedMemberSaved.lock();
    auto class_Split_MarkedMemberNotSavedOrLoaded_Ptr = class_Split_MarkedMemberNotSavedOrLoaded.lock();
    auto class_Split_WrongOrder_TypeOk_Ptr = class_Split_WrongOrder_TypeOk.lock();
    auto class_Split_WrongOrder_TypeWrong_Ptr = class_Split_WrongOrder_TypeWrong.lock();

    EXPECT_TRUE(class_Split_AllMembersSerialized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_AllMembersSerialized_Ptr->NumberOfErrors(), 0);
    EXPECT_TRUE(class_Split_AllMembersSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_AllMembersSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_AllMembersSerialized_NoMacro_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_AllMembersSerialized_NoMacro_Ptr->NumberOfErrors(), 0);
    EXPECT_TRUE(class_Split_AllMembersSerialized_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_FALSE(class_Split_AllMembersSerialized_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_OneMemberNotSaved_NoMacro_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_FALSE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_NonSplit_AllMembersSerialized_SplitMemberExists_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_NonSplit_AllMembersSerialized_SplitMemberExists_Ptr->NumberOfErrors(), 0);
    EXPECT_FALSE(class_NonSplit_AllMembersSerialized_SplitMemberExists_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_FALSE(class_NonSplit_AllMembersSerialized_SplitMemberExists_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_OneMemberNotSaved_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_OneMemberNotSaved_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_OneMemberNotSaved_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Split_OneMemberNotSaved_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_OneMemberNotSaved_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_OneMemberNotLoaded_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_OneMemberNotLoaded_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_OneMemberNotLoaded_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Split_OneMemberNotLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_OneMemberNotLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_FALSE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->NumberOfErrors(), 2);
    EXPECT_TRUE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->NumberOfErrors(), 2);
    EXPECT_TRUE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_WrongOrder_TypeOk_Ptr->HasError(SerializationErrorFlag::Error_SaveLoadOrderMismatched));
    EXPECT_FALSE(class_Split_WrongOrder_TypeOk_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_WrongOrder_TypeOk_Ptr->NumberOfErrors(), 2); // Ideally this should just be 1 error, but that will need work.
    EXPECT_TRUE(class_Split_WrongOrder_TypeOk_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_WrongOrder_TypeOk_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_WrongOrder_TypeWrong_Ptr->HasError(SerializationErrorFlag::Error_SaveLoadOrderMismatched));
    EXPECT_FALSE(class_Split_WrongOrder_TypeWrong_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_WrongOrder_TypeWrong_Ptr->NumberOfErrors(), 2); // Ideally this should just be 1 error, but that will need work.
    EXPECT_TRUE(class_Split_WrongOrder_TypeWrong_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_WrongOrder_TypeWrong_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));
}

TEST_F(SH_Tests, SplitDirectionalAnalysis) {
    SerializationContext::SetDiagnosticsReporter(std::make_unique<TestDiagnosticReporter>("SplitSingleTUTests.log"));
    std::string file = "./projects/split/split_directional.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    EXPECT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableClassInfoMediator::FlattenSerializableContainer();

    EXPECT_EQ(serializable_classes.size(), 9);

    auto class_Split_AllMembersSerialized = GetClassFromVector(serializable_classes, "Split_AllMembersSerialized_Directional");
    auto class_Split_AllMembersSerialized_NoMacro = GetClassFromVector(serializable_classes, "Split_AllMembersSerialized_Directional_NoMacro");
    auto class_Split_OneMemberNotSaved_NoMacro = GetClassFromVector(serializable_classes, "Split_OneMemberNotSaved_Directional_NoMacro");
    auto class_Split_OneMemberNotSaved = GetClassFromVector(serializable_classes, "Split_OneMemberNotSaved_Directional");
    auto class_Split_OneMemberNotLoaded = GetClassFromVector(serializable_classes, "Split_OneMemberNotLoaded_Directional");
    auto class_Split_UnmarkedMemberSaved = GetClassFromVector(serializable_classes, "Split_UnmarkedMemberSavedAndLoaded_Directional");
    auto class_Split_MarkedMemberNotSavedOrLoaded = GetClassFromVector(serializable_classes, "Split_MarkedMemberNotSavedOrLoaded_Directional");
    auto class_Split_WrongOrder_TypeOk = GetClassFromVector(serializable_classes, "Split_WrongOrder_TypeOk_Directional");
    auto class_Split_WrongOrder_TypeWrong = GetClassFromVector(serializable_classes, "Split_WrongOrder_TypeWrong_Directional");

    EXPECT_FALSE(class_Split_AllMembersSerialized.expired());
    EXPECT_FALSE(class_Split_AllMembersSerialized_NoMacro.expired());
    EXPECT_FALSE(class_Split_OneMemberNotSaved_NoMacro.expired());
    EXPECT_FALSE(class_Split_OneMemberNotSaved.expired());
    EXPECT_FALSE(class_Split_OneMemberNotLoaded.expired());
    EXPECT_FALSE(class_Split_UnmarkedMemberSaved.expired());
    EXPECT_FALSE(class_Split_MarkedMemberNotSavedOrLoaded.expired());
    EXPECT_FALSE(class_Split_WrongOrder_TypeOk.expired());
    EXPECT_FALSE(class_Split_WrongOrder_TypeWrong.expired());

    auto class_Split_AllMembersSerialized_Ptr = class_Split_AllMembersSerialized.lock();
    auto class_Split_AllMembersSerialized_NoMacro_Ptr = class_Split_AllMembersSerialized_NoMacro.lock();
    auto class_Split_OneMemberNotSaved_NoMacro_Ptr = class_Split_OneMemberNotSaved_NoMacro.lock();
    auto class_Split_OneMemberNotSaved_Ptr = class_Split_OneMemberNotSaved.lock();
    auto class_Split_OneMemberNotLoaded_Ptr = class_Split_OneMemberNotLoaded.lock();
    auto class_Split_UnmarkedMemberSavedAndLoaded_Ptr = class_Split_UnmarkedMemberSaved.lock();
    auto class_Split_MarkedMemberNotSavedOrLoaded_Ptr = class_Split_MarkedMemberNotSavedOrLoaded.lock();
    auto class_Split_WrongOrder_TypeOk_Ptr = class_Split_WrongOrder_TypeOk.lock();
    auto class_Split_WrongOrder_TypeWrong_Ptr = class_Split_WrongOrder_TypeWrong.lock();

    EXPECT_TRUE(class_Split_AllMembersSerialized_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_AllMembersSerialized_Ptr->NumberOfErrors(), 0);
    EXPECT_TRUE(class_Split_AllMembersSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_AllMembersSerialized_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_AllMembersSerialized_NoMacro_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_AllMembersSerialized_NoMacro_Ptr->NumberOfErrors(), 0);
    EXPECT_TRUE(class_Split_AllMembersSerialized_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_FALSE(class_Split_AllMembersSerialized_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_OneMemberNotSaved_NoMacro_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_FALSE(class_Split_OneMemberNotSaved_NoMacro_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_OneMemberNotSaved_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_OneMemberNotSaved_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_OneMemberNotSaved_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Split_OneMemberNotSaved_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_OneMemberNotSaved_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_OneMemberNotLoaded_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_OneMemberNotLoaded_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_OneMemberNotLoaded_Ptr->NumberOfErrors(), 1);
    EXPECT_TRUE(class_Split_OneMemberNotLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_OneMemberNotLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasError(SerializationErrorFlag::Error_UnmarkedFieldSerialized));
    EXPECT_FALSE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->NumberOfErrors(), 2);
    EXPECT_TRUE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_UnmarkedMemberSavedAndLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasError(SerializationErrorFlag::Error_MarkedFieldNotSerialized));
    EXPECT_FALSE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->NumberOfErrors(), 2);
    EXPECT_TRUE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_MarkedMemberNotSavedOrLoaded_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_WrongOrder_TypeOk_Ptr->HasError(SerializationErrorFlag::Error_SaveLoadOrderMismatched));
    EXPECT_FALSE(class_Split_WrongOrder_TypeOk_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_WrongOrder_TypeOk_Ptr->NumberOfErrors(), 2); // Ideally this should just be 1 error, but that will need work.
    EXPECT_TRUE(class_Split_WrongOrder_TypeOk_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_WrongOrder_TypeOk_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));

    EXPECT_TRUE(class_Split_WrongOrder_TypeWrong_Ptr->HasError(SerializationErrorFlag::Error_SaveLoadOrderMismatched));
    EXPECT_FALSE(class_Split_WrongOrder_TypeWrong_Ptr->HasError(SerializationErrorFlag::Error_NoError));
    EXPECT_EQ(class_Split_WrongOrder_TypeWrong_Ptr->NumberOfErrors(), 2); // Ideally this should just be 1 error, but that will need work.
    EXPECT_TRUE(class_Split_WrongOrder_TypeWrong_Ptr->HasInfo(SerializationInfoFlag::Info_SplitIntrusiveSerialization));
    EXPECT_TRUE(class_Split_WrongOrder_TypeWrong_Ptr->HasInfo(SerializationInfoFlag::Info_UsesSplitMacro));
}