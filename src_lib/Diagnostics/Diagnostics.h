// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================

// ==================================
// Libtooling Headers
// ==================================
#include "clang/AST/DeclTemplate.h"

// ==================================
// Forward Declarations
// ==================================

void setupDiagnostics(clang::DiagnosticsEngine &Diag);

void reportDiagnostic(clang::DiagnosticsEngine &Diag, const std::string &message);