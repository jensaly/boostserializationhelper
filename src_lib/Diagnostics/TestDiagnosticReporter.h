#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <fstream>

// ==================================
// Internal Headers
// ==================================
#include <Diagnostics/IDiagnosticReporter.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

class TestDiagnosticReporter : public IDiagnosticReporter {
    std::ofstream file;

public:
    explicit TestDiagnosticReporter(std::string filename);

    ~TestDiagnosticReporter();

    void forward(std::vector<std::string> const& messages) override;
};
