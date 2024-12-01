#include <gtest/gtest.h>
#include <basic/basic_class.hpp>
#include <serializable_class.h>
#include <fstream>
#include <sstream>

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

CXXRecordDecl* GetClassFromVector(std::vector<CXXRecordDecl*> decls, std::string name) {
    auto it = std::find_if(decls.begin(), decls.end(), [name](const CXXRecordDecl* decl){
        return name == decl->getNameAsString();
    });
    return (it != decls.end()) ? *it : nullptr;
}

TEST(SH_BasicProject_Tests, BasicAnalysis) {
    std::string file = "./projects/basic/basic_class.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-E", std::string("-I/") + COMMON_DIR, "-x", "c++"};

    ASSERT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));

    auto serializable_classes = SerializableCXXRecordDeclStorage::GetClasses();

    ASSERT_EQ(serializable_classes.size(), 5);

    auto class_Basic_NotSerialized = GetClassFromVector(serializable_classes, "Basic_NotSerialized");
    auto class_Basic_SerializableWithoutFunction = GetClassFromVector(serializable_classes, "Basic_SerializableWithoutFunction");
    auto class_Basic_AllMembersSerialized = GetClassFromVector(serializable_classes, "Basic_AllMembersSerialized");
    auto class_Basic_OneMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_OneMemberNotSerialized");
    auto class_Basic_TaggedMemberNotSerialized = GetClassFromVector(serializable_classes, "Basic_TaggedMemberNotSerialized");
    auto class_Basic_UntaggedMemberSeralized = GetClassFromVector(serializable_classes, "Basic_UntaggedMemberSeralized");

    ASSERT_EQ(class_Basic_NotSerialized, nullptr);
    ASSERT_NE(class_Basic_SerializableWithoutFunction, nullptr);
    ASSERT_NE(class_Basic_AllMembersSerialized, nullptr);
    ASSERT_NE(class_Basic_OneMemberNotSerialized, nullptr);
    ASSERT_NE(class_Basic_TaggedMemberNotSerialized, nullptr);
    ASSERT_NE(class_Basic_UntaggedMemberSeralized, nullptr);

    ASSERT_FALSE(SerializableInClassAnalyzer::hasSerializeMethod(class_Basic_SerializableWithoutFunction));
    ASSERT_TRUE(SerializableInClassAnalyzer::hasSerializeMethod(class_Basic_AllMembersSerialized));
    ASSERT_TRUE(SerializableInClassAnalyzer::hasSerializeMethod(class_Basic_OneMemberNotSerialized));
    ASSERT_TRUE(SerializableInClassAnalyzer::hasSerializeMethod(class_Basic_TaggedMemberNotSerialized));
    ASSERT_TRUE(SerializableInClassAnalyzer::hasSerializeMethod(class_Basic_UntaggedMemberSeralized));

    auto method_Basic_AllMembersSerialized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_AllMembersSerialized);
    auto method_Basic_OneMemberNotSerialized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_OneMemberNotSerialized);
    auto method_Basic_TaggedMemberNotSerialized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_TaggedMemberNotSerialized);
    auto method_Basic_UntaggedMemberSeralized = SerializableInClassAnalyzer::getSerializeMethod(class_Basic_UntaggedMemberSeralized);

    ASSERT_TRUE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_AllMembersSerialized, class_Basic_AllMembersSerialized));
    ASSERT_TRUE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_OneMemberNotSerialized, class_Basic_OneMemberNotSerialized));
    ASSERT_FALSE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_TaggedMemberNotSerialized, class_Basic_TaggedMemberNotSerialized));
    ASSERT_TRUE(SerializableInClassAnalyzer::checkAllSerializeableInSerialize(method_Basic_UntaggedMemberSeralized, class_Basic_UntaggedMemberSeralized));
}