#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <iostream>

// ==================================
// Internal Headers
// ==================================
#include <Diagnositcs/IDiagnosticReporter.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

class TestDiagnosticReporter : public IDiagnosticReporter {
    std::ostream &output;

public:
    explicit TestDiagnosticReporter(std::ostream &output = std::cout) : output(output) {}

    void report(const DiagnosticMessage &message) override {
        output << message.toString() << std::endl;
    }
};
