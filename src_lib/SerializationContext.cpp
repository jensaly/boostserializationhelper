// ==================================
// Standard Library Headers
// ==================================
#include <iostream>

// ==================================
// Internal Headers
// ==================================
#include "SerializationContext.h"
#include <Diagnostics/IDiagnosticReporter.h>
#include <Mediation/SerializableClassInfoMediator.h>
#include <SerializationInfo/SerializableClassInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================


std::unique_ptr<IDiagnosticReporter> SerializationContext::m_reporter = nullptr;

void SerializationContext::SetDiagnosticsReporter(std::unique_ptr<IDiagnosticReporter> reporter) {
    m_reporter = std::move(reporter);
}

void SerializationContext::Reset() {
    m_reporter.reset();
}

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

void SerializationContext::Log() {
    auto& serializableClasses = SerializableClassInfoMediator::serializables;
    std::vector<std::string> output;
    for (auto& [decl, info] : serializableClasses) {
        info->Log(output);
    }
    m_reporter->forward(output);
}