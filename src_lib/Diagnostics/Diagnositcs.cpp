// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <Diagnostics/Diagnostics.h>

// ==================================
// Libtooling Headers
// ==================================
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>

// ==================================
// Forward Declarations
// ==================================

void setupDiagnostics(clang::DiagnosticsEngine &Diag) {
    auto DiagOpts = std::make_shared<clang::DiagnosticOptions>();
    // Use TextDiagnosticPrinter to print diagnostics to the terminal
    auto DiagClient = std::make_unique<clang::TextDiagnosticPrinter>(
        llvm::errs(), DiagOpts.get());
    
    Diag.setClient(DiagClient.release(), /*shouldOwnClient=*/true);
}
