#pragma once

// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================

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
    virtual void report(const DiagnosticMessage &message) = 0;
};
