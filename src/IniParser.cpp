#include <iostream>
#include <fstream>
#include <exception>
#include "IniParser.h"

using namespace std;

IniParser::IniParser() {
    // comments can either start with ; or #:  ;;;this is a coomment
    regexComment = regex("(\\s*?)(;|#)(.*)", regex::ECMAScript);
    
    // sections are placed between square brackets:  [Section.SubSection... and so on]
    regexSection = regex("(\\s*?)\\[[^\\]\\r\\n]+](\\s*?)", regex::ECMAScript);

    // operator = is used to assign values to keys
    regexKeyValueAssigment = regex("(\\s*?)(_*[a-z|A-Z][a-z|A-Z|0-9|_]*)([\\s|\\t]*?)=([^;\\r\\n]*)", regex::ECMAScript);

    strCurrentSection = "";
}

IniParser::IniParser(const string& strFileName) {
    IniParser();                    // delegate the default constructor
    updateFromFile(strFileName);    // update
}

int IniParser::updateFromFile(const string& strFileName) {
    
    // check for empty file name
    if (strFileName.empty())
        throw invalid_argument("The input file name is empty! Give me something!");
    
    ifstream ifs(strFileName, std::ifstream::in);
    
    // check if file was open
    if (ifs.fail())
        throw invalid_argument("Unable to open the input file");

    cout << "reading: " << strFileName << endl;
    while (!ifs.eof()) {
        string strLine;
        
        // read line
        getline(ifs, strLine);

        //skip empty lines
        if (strLine.empty()) {
            cout << "read: empty line" << endl;
            continue;
        }

        cout << "read: " << strLine << endl;

        smatch matcher;
        try{
            // skip comments
            if(regex_match(strLine, matcher, regexComment)) {
                cout << "matched comment: " << trim(matcher[0]) << endl;
                continue;
            }

            if(regex_match(strLine, matcher, regexSection)) {
                cout << "matched section: " << trim(matcher[0]) << endl;
                // reset prevews section if any
                // process the new section here
                continue;
            }

            if(regex_match(strLine, matcher, regexKeyValueAssigment)) {
                cout << "matched key value assigment: " << trim(matcher[0]) << endl;
                // process the section here 
                continue;
            }
        } catch (const regex_error& ex) {
            cout << "internal error, regex exception";            
            return -1;
        }

        cout << "invalid line: " << strLine << endl;
    }

    cout << "eof:" << strFileName << endl;

    ifs.close();
    return 0;
}

string IniParser::trim(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return string(it, rit.base());
}

const string& IniParser::getValue(const string& strKey) {
    return getValue("", strKey);
}

const string& IniParser::getValue(const string& strSection, const string& strKey) {
    return strKey;
}

void IniParser::handleKeyValueAssigment(const string& strKeyValueAssigment) {

}