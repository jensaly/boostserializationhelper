// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <Diagnostics/TestDiagnosticReporter.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

TestDiagnosticReporter::TestDiagnosticReporter(std::string filename) {
    file.open(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open() || !file.good()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}

TestDiagnosticReporter::~TestDiagnosticReporter() {
    if (file.is_open()) {
        file.close();
    }
}

void TestDiagnosticReporter::forward(std::vector<std::string> const& messages) {
    if (!file.is_open() || !file.good()) {
        throw std::runtime_error("File is not open or not in good state.");
    }


    for (const auto& message : messages) {
        file << message << "\n";
        if (!file.good()) {
            throw std::runtime_error("Failed to write to file.");
        }
    }
}