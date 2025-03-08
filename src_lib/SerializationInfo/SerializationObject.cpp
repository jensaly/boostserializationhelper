// ==================================
// Standard Library Headers
// ==================================
#include <string>

// ==================================
// Internal Headers
// ==================================
#include <SerializationInfo/SerializationObject.h>

// ==================================
// Libtooling Headers
// ==================================

// ==================================
// Forward Declarations
// ==================================

SerializationObject::SerializationObject(std::string declName, std::string filename, unsigned int line, unsigned int column)
        : m_name{declName}, m_filename{filename}, m_line{line}, m_column{column} 
{

}