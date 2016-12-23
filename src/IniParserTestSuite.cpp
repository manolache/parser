#include <iostream>
#include <assert.h>

#include "IniParserTestSuite.h"


using namespace std;

IniParserTestSuite::IniParserTestSuite(string strEmptyFile, string strFirstFile, string strUpdateFile)
    : m_strEmptyFile(strEmptyFile),
      m_strFirstFile(strFirstFile),
      m_strUpdateFile(strUpdateFile)
{
    // helps debugging the test suite
    assert (strEmptyFile.length() != 0 
            && strFirstFile.length() != 0
            && strUpdateFile.length() != 0);

    // build the ini parser with skip invalid lines capabilities
    m_iniParser = IniParser(true); 
}

bool IniParserTestSuite::runTests() {
    
    m_iniParser.reset();

    bool bReturn = testInvalidPath();
    bReturn = bReturn && testEmptyFile();
    bReturn = bReturn && testKeyValueAssigments();
    bReturn = bReturn && testKeyValueAssigmentsUpdate();

    return bReturn;
}

bool IniParserTestSuite::testInvalidPath() {
    cout << "Testing the .INI parser with an invalid file path...\n";
    try {
        // try with empty path
        int nReturn = m_iniParser.updateFromFile("");
        if (0 == nReturn) {
            cout << "[Failed]\n";
            return false;             
        }

        // try with invalid path
        nReturn = m_iniParser.updateFromFile(" this is still an invalid file path ");
        if (0 == nReturn) {
            cout << "[Failed]\n";
            return false;             
        }
    } catch (invalid_argument ex) {
        cout << ex.what() << endl;
        cout << "[Passed]\n";
        return true; 
    }

    cout << "[Failed]\n";
    return false;
}

bool IniParserTestSuite::testEmptyFile() {
    cout << "Testing the .INI parser with an empty file...\n";
    
    // hold the current no of values
    size_t noOfValues = m_iniParser.getNoOfValues();

    try {
        // try with empty file
        int nReturn = m_iniParser.updateFromFile(m_strEmptyFile);
        if (0 != nReturn) {
            cout << "[Failed]\n";
            return false;             
        }
    } catch (invalid_argument ex) {
        cout << ex.what() << endl;
        cout << "[Failed]\n";
        return false; 
    } catch (IniParser::invalid_format_exception ex) {
        cout << ex.what() << endl;
        cout << "[Failed]\n";
        return false; 
    }

    // empty files should not affect the number of values
    if (noOfValues != m_iniParser.getNoOfValues()){
        cout << "[Failed]\n";
        return false;
    } 

    cout << "[Passed]\n";
    return true;
}

bool IniParserTestSuite::testKeyValueAssigments() {
    cout << "Testing the .INI parser with the first file...\n";
    
    // hold the current no of values
    size_t noOfValues = m_iniParser.getNoOfValues();
    
    // expects 0 values
    assert (0 == noOfValues);  

    try {
        // parse the first file
        int nReturn = m_iniParser.updateFromFile(m_strFirstFile);
        if (0 != nReturn) {
            cout << "[Failed]\n";
            return false;             
        }
    } catch (invalid_argument ex) {
        cout << ex.what() << endl;
        cout << "[Failed]\n";
        return false; 
    } catch (IniParser::invalid_format_exception ex) {
        cout << ex.what() << endl;
        cout << "[Failed]\n";
        return false; 
    }

    noOfValues = m_iniParser.getNoOfValues();
    
    // expects 9 values
    assert (9 == noOfValues);  
    
    try{
        if (m_iniParser.getString("another section", "key") != "value4") {
            cout << "[Failed]\n";
            return false; 
        }
            
        if (m_iniParser.getString("details.about", "city") != "bucharest") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("details.about", "country") != "romania") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getBool("details.about", "isNice")){
            cout << "[Failed]\n";
            return false;         
        }

        if (m_iniParser.getString("details.about", "lastname") != "manolache") {
            cout << "[Failed]\n";
            return false; 
        }
        
        if (m_iniParser.getString("details.about", "name") != "ionut") {
            cout << "[Failed]\n";
            return false; 
        }

        // river
        
        if (m_iniParser.getInt("section", "key") != 7) {
            cout << "[Failed]\n";
            return false; 
        }

    } catch (IniParser::invalid_format_exception ex) {
        cout << ex.what() << endl;
        cout << "[Failed]\n";
        return false;         
    } catch (IniParser::no_such_key_exception ex) {
        cout << ex.what() << endl;
        cout << "[Failed]\n";
        return false;         
    }

    cout << "[Passed]\n";
    return true;
}

bool IniParserTestSuite::testKeyValueAssigmentsUpdate() {
    return true;
}