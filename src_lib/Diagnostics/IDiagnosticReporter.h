#pragma once

// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <vector>
#include <string>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

class DiagnosticMessage;

class IDiagnosticReporter {
public:
    virtual ~IDiagnosticReporter() = default;
    virtual void forward(std::vector<std::string> const& messages) = 0;
};
