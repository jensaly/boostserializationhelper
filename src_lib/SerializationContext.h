#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <memory>

// ==================================
// Internal Headers
// ==================================

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================
class IDiagnosticReporter;

class SerializationContext {
public:
    static void Mediate();

    static void Analyze();

    static void Log();

    static void SetDiagnosticsReporter(std::unique_ptr<IDiagnosticReporter> reporter);

    static void Reset();

private:
    static std::unique_ptr<IDiagnosticReporter> m_reporter;

    static void ResolveNonIntrusiveSerializeMethods();
};
