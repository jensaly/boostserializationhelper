#include <gtest/gtest.h>
#include <basic/basic_class.h>
#include <matcher.h>

TEST(SH_BasicProject_Tests, BasicAnalysis) {
    // You would load files from project1 here and analyze them
    std::string file = "./projects/basic/basic_class.h";
    std::make_unique<FindSerializableClassAction>();
    clang::tooling::runToolOnCode(std::make_unique<FindSerializableClassAction>(), file.c_str());
}
