#include <iostream>
#include <assert.h>

#include "IniParserTestSuite.h"


using namespace std;

IniParserTestSuite::IniParserTestSuite(const string& strEmptyFile, 
									   const string& strFirstFile,
									   const string& strUpdateFile)
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
    
    m_iniParser.clear();

    bool bReturn = testInvalidPath();
    bReturn = bReturn && testEmptyFile();
    bReturn = bReturn && testKeyValueAssigments();
    bReturn = bReturn && testKeyValueAssigmentsUpdate();
    bReturn = bReturn && testNoSuchKeyException();
    bReturn = bReturn && testInvalidFormatException();
    bReturn = bReturn && testClear();

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
    size_t noOfValues = m_iniParser.size();

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
    if (noOfValues != m_iniParser.size()){
        cout << "[Failed]\n";
        return false;
    } 

    cout << "[Passed]\n";
    return true;
}

bool IniParserTestSuite::testKeyValueAssigments() {
    cout << "Testing the .INI parser with the first file...\n";
       
    // expects 0 values
    if (0 != m_iniParser.size()) {
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
    if (9 != m_iniParser.size()) {
        cout << "[Failed]\n";
        return false;             
    }
    
    try {          
        if (m_iniParser.getString("city", "details.about") != "bucharest") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("country", "details.about") != "romania") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getBool("isNice", "details.about")){
            cout << "[Failed]\n";
            return false;         
        }

        if (m_iniParser.getString("lastname", "details.about") != "manolache") {
            cout << "[Failed]\n";
            return false; 
        }
        
        if (m_iniParser.getString("name", "details.about") != "ionut") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getInt("key") != 7) {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("lake") != "\"\"") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("river") != "") {
            cout << "[Failed]\n";
            return false; 
        }

        if (m_iniParser.getString("key", "section") != "some string with spaces") {
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
    if (9 != m_iniParser.size()) {
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
    if (10 != m_iniParser.size()) {
        cout << "[Failed]\n";
        return false;             
    }

    try {
        if (m_iniParser.getString("company") != "eset") {
            cout << "[Failed]\n";
            return false; 
        }

        if (!m_iniParser.getBool("isNice", "details.about")){
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
        if (m_iniParser.getString("some key", "some section") != "some value") {
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
        if (m_iniParser.getInt("name", "details.about") != 999) {
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


bool IniParserTestSuite::testClear(){
    cout << "Testing the clear operation...\n";
	
	m_iniParser.clear();

	if (m_iniParser.size() != 0) {
        cout << "[Failed]\n";
        return false;         	
	}

	cout << "[Passed]\n";
	return true;         	
}