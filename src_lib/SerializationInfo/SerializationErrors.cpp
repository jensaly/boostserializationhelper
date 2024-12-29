// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializationErrors.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

SerializationError_MarkedFieldNotSerialized::SerializationError_MarkedFieldNotSerialized(
    std::string filenameDecl, unsigned int declLine, unsigned int declColumn,
    std::string filenameSave, unsigned int saveLine, unsigned int saveColumn,
    std::string filenameLoad, unsigned int loadLine, unsigned int loadColumn
)
        : m_filenameDecl{filenameDecl}, m_declLine{declLine}, m_declColumn{declColumn},
        m_filenameSave{filenameSave}, m_saveLine{saveLine}, m_saveColumn{saveColumn},
        m_filenameLoad{filenameLoad}, m_loadLine{loadLine}, m_loadColumn{loadColumn},
        SerializationError(SerializationErrorFlag::Error_MarkedFieldNotSerialized)
{
    m_split = true;
}

// For normal serialization
SerializationError_MarkedFieldNotSerialized::SerializationError_MarkedFieldNotSerialized(
    std::string filenameDecl, unsigned int declLine, unsigned int declColumn,
    std::string filenameSave, unsigned int saveLine, unsigned int saveColumn
)
        : m_filenameDecl{filenameDecl}, m_declLine{declLine}, m_declColumn{declColumn},
        m_filenameSave{filenameSave}, m_saveLine{saveLine}, m_saveColumn{saveColumn},
        m_filenameLoad{""}, m_loadLine{UINT32_MAX}, m_loadColumn{UINT32_MAX},
        SerializationError(SerializationErrorFlag::Error_MarkedFieldNotSerialized)
{
    m_split = false;
}

std::string SerializationError_MarkedFieldNotSerialized::ToString() const {
    return "Testing";
}