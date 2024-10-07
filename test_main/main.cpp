#include "basic.h"

int main(int argc, char **argv) {
    //clang::tooling::runToolOnCode(std::make_unique<FindSerializableClassAction>(), argv[1]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
