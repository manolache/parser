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
    bReturn = bReturn && testNoSuchKeyException();
    bReturn = bReturn && testInvalidFormatException();
    
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
       
    // expects 0 values
    if (0 != m_iniParser.getNoOfValues()) {
        cout << "[Failed]\n";
        return false;             
    }

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
   
    // expect 9 values
    if (9 != m_iniParser.getNoOfValues()) {
        cout << "[Failed]\n";
        return false;             
    }
    
    try {          
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

        if (m_iniParser.getInt("", "key") != 7) {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("", "lake") != "\"\"") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("", "river") != "") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("section", "key") != "some string with spaces") {
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
    cout << "Testing the .INI parser with the update file...\n";
    
    // expect 9 values from the first file
    if (9 != m_iniParser.getNoOfValues()) {
        cout << "[Failed]\n";
        return false;             
    }

    try {
        // parse the first file
        int nReturn = m_iniParser.updateFromFile(m_strUpdateFile);
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
    
    // expects 10 values
    if (10 != m_iniParser.getNoOfValues()) {
        cout << "[Failed]\n";
        return false;             
    }

    try {
        if (m_iniParser.getString("", "company") != "eset") {
            cout << "[Failed]\n";
            return false; 
        }

        if (!m_iniParser.getBool("details.about", "isNice")){
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

bool IniParserTestSuite::testNoSuchKeyException() {
    
    cout << "Testing the no scuh key exception...\n";

    try {
        if (m_iniParser.getString("some section", "some key") != "some value") {
            cout << "[Failed]\n";
            return false; 
        }
    } catch (IniParser::no_such_key_exception ex) {
        cout << ex.what() << endl;
        cout << "[Passed]\n";
        return true;         
    }

    cout << "[Failed]\n";
    return false;
}

bool IniParserTestSuite::testInvalidFormatException() {
    cout << "Testing the invalid format exception...\n";

    try {
        if (m_iniParser.getInt("details.about", "name") != 999) {
            cout << "[Failed]\n";
            return false; 
        }
    } catch (IniParser::invalid_format_exception ex) {
        cout << ex.what() << endl;
        cout << "[Passed]\n";
        return true;         
    } catch (IniParser::no_such_key_exception ex) {
        cout << ex.what() << endl;
        cout << "[Failed]\n";
        return false;         
    }

    cout << "[Failed]\n";
    return false;         
}
