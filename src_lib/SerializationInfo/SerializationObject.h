#pragma once

// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

class SerializationObject {
    std::string m_name;
    std::string m_filename{"NOTAFILE"};
    unsigned int m_line = UINT32_MAX;
    unsigned int m_column = UINT32_MAX;
public:
    SerializationObject() = delete;

    SerializationObject(std::string declName, std::string filename, unsigned int line, unsigned int column);

    std::string GetName() const { return m_name; }

    std::string GetFilename() const { return m_filename; }

    unsigned int GetLine() const { return m_line; }

    unsigned int GetColumn() const { return m_column; }

    virtual ~SerializationObject() {}
};