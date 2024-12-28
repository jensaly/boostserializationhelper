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

struct DiagnosticMessage {
    Severity severity;
    std::string message;
    std::string file;
    unsigned line = 0;
    unsigned column = 0;

    // Optional: Add helper methods for formatting
    std::string toString() const {
        return "[" + severityToString(severity) + "] " + file + ":" + 
               std::to_string(line) + ":" + std::to_string(column) + " - " + message;
    }

    static std::string severityToString(Severity sev) {
        switch (sev) {
        case Severity::Info: return "Info";
        case Severity::Warning: return "Warning";
        case Severity::Error: return "Error";
        }
        return "Unknown";
    }
};
