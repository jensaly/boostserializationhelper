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

class SerializationContext {
public:
    static void Mediate();

    static void Analyze();

private:
    static void ResolveNonIntrusiveSerializeMethods();
};
