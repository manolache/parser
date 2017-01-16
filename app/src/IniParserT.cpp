#include <exception>

#include "IniParser.h"

using namespace std;

// this is a specialisation of the template getter for 'string' type
// there is no point to drag a string through a string stream and get it back after all
// even more, the >> operator will stop at the first white space when reading a string
template <>
string IniParser::getValueT(const string& strKey, const string& strSection) const {
    return getValue(strKey, strSection);
}

// this is a specialisation of the template getter for 'bool' type
template <>
bool IniParser::getValueT(const string& strKey, const string& strSection) const {
    string strValue = getValue(strKey, strSection);

    if (strValue == "true" ) // TODO: handle capital letters
        return true;
    
    if (strValue == "false") // TODO: handle capital letters
        return false;

    throw IniParser::invalid_format_exception("The value assigned to " + strKey + " is not a boolean!");    
}
