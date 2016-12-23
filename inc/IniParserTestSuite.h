#include <string>

#include "IniParser.h"

using namespace std;

// this is a lasy way to excersite the IniParser
class IniParserTestSuite {
public: // methods
    IniParserTestSuite(string strEmptyFile, string strFirstFile, string strUpdateFile);
    bool runTests();

private: // unit tests
    bool testInvalidPath();
    bool testEmptyFile();
    bool testKeyValueAssigments();
    bool testKeyValueAssigmentsUpdate();
    
private: // atributes
    IniParser   m_iniParser;
    string      m_strEmptyFile;
    string      m_strFirstFile; 
    string      m_strUpdateFile;
};