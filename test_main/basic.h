#include <gtest/gtest.h>
#include <basic/basic_class.h>
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
    // You would load files from project1 here and analyze them
    std::string file = "./projects/basic/basic_class.h";
    std::string code = GetCodeFromSourceFile(file);
    std::vector<std::string> args{"-fsyntax-only"};

    clang::tooling::runToolOnCodeWithArgs(std::make_unique<FindSerializableClassAction>(), code, args, file);
}
