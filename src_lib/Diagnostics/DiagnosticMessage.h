#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

enum class Severity { Info, Warning, Error };

class DiagnosticMessage {
public:
    std::string serializationErrorToString()
};
