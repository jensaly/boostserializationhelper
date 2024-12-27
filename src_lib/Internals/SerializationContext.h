#pragma once

#include <string>
#include <vector>

class SerializationContext {
public:
    static void Mediate();

    static void Analyze();

private:
    static void ResolveNonIntrusiveSerializeMethods();
};
