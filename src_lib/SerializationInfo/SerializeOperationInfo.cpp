// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializeOperationInfo.h>

// ==================================
// Libtooling Headers
// ==================================

// ================================== 
// Forward Declarations
// ==================================


SerializeOperationInfo::SerializeOperationInfo(
        std::string operationName, std::string operationFilename, unsigned int operationLine, unsigned int operationColumn,
        std::string declName, std::string declFilename, unsigned int declLine, unsigned int declColumn
)
        : SerializationObject(declName, declFilename, declLine, declColumn),
        m_operationName{operationName}, m_operationFilename{operationFilename}, m_operationLine{operationLine}, m_operationColumn{operationColumn}
{

}

std::string SerializeOperationInfo::GetOperationName() const {
        return m_operationName;
}

std::string SerializeOperationInfo::GetOperationFilename() const {
        return m_operationFilename;
}

unsigned int SerializeOperationInfo::GetOperationLine() const {
        return m_operationLine;
}

unsigned int SerializeOperationInfo::GetOperationColumn() const {
        return m_operationColumn;
}