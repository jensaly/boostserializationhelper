#pragma once

#include <memory>

class SerializableClassInfo;
class SerializableFieldInfo;
class SerializeOperationInfo;
class SerializeFunctionInfo;

using SerializableClassName = std::string;
using SerializableClassInfoPtr = std::shared_ptr<SerializableClassInfo>;
using SerializableClassInfoWeakPtr = std::weak_ptr<const SerializableClassInfo>;
using SerializeFunctionInfoPtr = std::shared_ptr<SerializeFunctionInfo>;
using SerializeFunctionInfoWeakPtr = std::weak_ptr<const SerializeFunctionInfo>;