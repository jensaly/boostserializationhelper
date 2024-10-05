#pragma once
// STL Includes
// ====================================================== //
#include <fstream>
#include <string>
#include <sstream>

// LibTooling Includes
// ====================================================== //

// GTest Includes
// ====================================================== //

const std::string loadTestSourceFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}