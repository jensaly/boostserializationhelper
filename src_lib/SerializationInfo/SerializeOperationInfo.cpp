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

SerializeOperationInfo::SerializeOperationInfo(std::string operationName, std::string filename, unsigned int line, unsigned int column)
        : SerializationObject(operationName, filename, line, column)
{

}