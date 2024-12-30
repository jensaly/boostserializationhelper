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
using SerializeOperationInfoPtr = std::shared_ptr<SerializeOperationInfo>;
using SerializeOperationInfoWeakPtr = std::weak_ptr<const SerializeOperationInfo>;
using SerializableFieldInfoInfoPtr = std::shared_ptr<SerializableFieldInfo>;
using SerializableFieldInfoInfoWeakPtr = std::weak_ptr<const SerializableFieldInfo>;