// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include "SerializationContext.h"
#include <Mediation/SerializableClassInfoMediator.h>
#include <SerializationInfo/SerializableClassInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

/// @brief 
void SerializationContext::Mediate() {
    ResolveNonIntrusiveSerializeMethods();
}

void SerializationContext::ResolveNonIntrusiveSerializeMethods() {
    auto& serializableClasses = SerializableClassInfoMediator::serializables;
    auto& freeSerializables = SerializeFunctionInfoMediator::serializables;
    for (auto& [decl, info] : serializableClasses) {
        if (info->HasSerializeMethod()) {
            // Already has serialize-method resolved during discovery.
            continue;
        }
        auto it = freeSerializables.find(decl);
        if (it == freeSerializables.end()) {
            // Could not find serialize method by mediation.
            // Error no serialize method
            info->SetSerializeMethodInfo(nullptr);
            continue;
        }
        info->SetSerializeMethodInfo(it->second);
    }
}

void SerializationContext::Analyze() {
    auto& serializableClasses = SerializableClassInfoMediator::serializables;
    for (auto& [decl, info] : serializableClasses) {
        info->RunSerializeMethodAnalysis();
    }
}