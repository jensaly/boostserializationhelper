// ==================================
// Standard Library Headers
// ==================================

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializeFunctionInfo.h>
#include <SerializationInfo/SerializeOperationInfo.h>
#include <SerializationInfo/SerializableClassInfo.h>
#include <SerializationInfo/SerializableFieldInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

SerializeFunctionInfo::SerializeFunctionInfo(std::string className, std::string filename, unsigned int line, unsigned int column)
        : SerializationObject(className, filename, line, column)
{

}

SerializeFunctionInfo::~SerializeFunctionInfo() {

}

void SerializeFunctionInfo::RunChecks(SerializableClassInfoPtr classInfo) {
    SerializationErrorFlag flag;
    auto methodContents = GetFields();
    auto classFields = classInfo->GetFields();

    for (auto& field : classFields) {
        auto operation_it = std::find_if(methodContents.begin(), methodContents.end(), [&](SerializeOperationInfoPtr& operationInfo){ 
            return field->GetName() == operationInfo->GetName();
        });
        if (operation_it == methodContents.end()) {
            auto& operation = *operation_it;
            // No serialize operations could be matched to the content of the class definition
            std::unique_ptr<SerializationError> error = std::make_unique<SerializationError_MarkedFieldNotSerialized>(
                *field, *this, classInfo
            );
            classInfo->SetError(std::move(error));
        }
    }

    for (auto& operationInfo : methodContents) {
        auto field_it = std::find_if(classFields.begin(), classFields.end(), [&](SerializableFieldInfoPtr& fieldInfo){ 
            return operationInfo->GetName() == fieldInfo->GetName();
        });
        if (field_it == classFields.end()) {
            std::unique_ptr<SerializationError> error = std::make_unique<SerializationError_UnmarkedFieldSerialized>(
                *operationInfo, *this, classInfo
            );
            classInfo->SetError(std::move(error));
        }
    }
}

void SerializeFunctionInfo::AddSerializableField(SerializeOperationInfoPtr&& operationInfo) {
    m_operationsInfo.push_back(std::move(operationInfo));
}

void SerializeFunctionInfo::SetError(SerializationErrorFlag error) {
    m_errorFlags |= error;
}

bool SerializeFunctionInfo::HasError(SerializationErrorFlag error) const {
    if (error != SerializationErrorFlag::Error_NoError) 
        return (m_errorFlags & error) != SerializationErrorFlag::Error_NoError;
    return true;
}

void SerializeFunctionInfo::SetSplitsInternally(bool splitsInternally) {
    m_splitsInternally = splitsInternally;
}

bool SerializeFunctionInfo::SplitsInternally() const {
    return m_splitsInternally;
}

SerializeFunctionInfo_Intrusive::SerializeFunctionInfo_Intrusive(std::string funcName, std::string filename, unsigned int line, unsigned int column)
        : SerializeFunctionInfo(funcName, filename, line, column)
{

}

SerializeFunctionInfo_NonIntrusive::SerializeFunctionInfo_NonIntrusive(std::string funcName, std::string filename, unsigned int line, unsigned int column)
        : SerializeFunctionInfo(funcName, filename, line, column)
{
    
}

SplitFunctionInfo_Intrusive::SplitFunctionInfo_Intrusive(SerializeFunctionInfoPtr&& save, SerializeFunctionInfoPtr&& load)
{
    m_save = std::move(save);
    m_load = std::move(load);
}

SplitFunctionInfo_Intrusive::~SplitFunctionInfo_Intrusive() {

}

void SplitFunctionInfo_Intrusive::RunChecks(SerializableClassInfoPtr classInfo) {
    m_load->RunChecks(classInfo); // This checks the load function by itself, not the order of the operations
    m_save->RunChecks(classInfo); // This checks the save function by itself, not the order of the operations

    auto loadFields = m_load->GetFields();
    auto saveFields = m_save->GetFields();

    // Basic order check
    // If the number of serialize operations is uneven, we nonetheless look at those which are already present. An error is raised if an issue exists within this
    for (int i = 0; i < loadFields.size() && i < saveFields.size(); i++) {
        auto lf = loadFields[i];
        auto sf = saveFields[i];

        if (lf->GetName() != sf->GetName()) {
            std::unique_ptr<SerializationError> error = std::make_unique<SerializationError_SaveLoadOrderMismatched>(
                *lf, *sf, *m_load, *m_save, classInfo
            );
            classInfo->SetError(std::move(error));
        }
    }
}