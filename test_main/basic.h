#include <gtest/gtest.h>
#include <basic/basic_class.hpp>
#include <matcher.h>
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

TEST(SH_BasicProject_Tests, BasicAnalysis) {
    std::string file = "./projects/basic/basic_class.hpp";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-fsyntax-only", "-I/home/jens/Documents/repos/serialization_helper/common", "-x", "c++"};

    ASSERT_TRUE(clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file));
}
