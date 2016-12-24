#include <iostream>
#include <fstream>
#include <exception>
#include <assert.h>

#include "IniParser.h"

using namespace std;

IniParser::IniParser(bool skipInvalidLines)
{
    // comments can either start with ; or #:  ;;;this is a comment
    regexComment = regex("(\\s*?)(;|#)(.*)", regex::ECMAScript);

    // sections are placed between square brackets:  [Section.SubSection... and so on]
    regexSection = regex("(\\s*?)\\[[^\\]\\r\\n]+](\\s*?)", regex::ECMAScript);

    // operator = is used to assign values to keys
    regexKeyValueAssigment = regex("(\\s*?)(_*[a-z|A-Z][a-z|A-Z|0-9|_]*)([\\s|\\t]*?)=([^;\\r\\n]*)", regex::ECMAScript);

    bSkipInvalidLines = skipInvalidLines;

    reset();
}

// just delegate the default constructor and process the file
IniParser::IniParser(const string &strFileName, bool skipInvalidLines) : IniParser(skipInvalidLines)
{
    updateFromFile(strFileName);
}

int IniParser::updateFromFile(const string &strFileName)
{
    // check for empty file name
    if (strFileName.empty())
        throw invalid_argument("The input file name is empty!");

    ifstream ifs(strFileName, std::ifstream::in);

    // check if file was open
    if (ifs.fail())
        throw invalid_argument("Unable to open the input file " + strFileName);

    // start with an empty section
    strCurrentSection = "";

    logInfo("reading " + strFileName);
    while (!ifs.eof())
    {
        string strLine;

        // read line
        getline(ifs, strLine);

        //skip empty lines
        if (strLine.empty())
        {
            logInfo("matched empty line, skipping...");
            continue;
        }

        smatch matcher;
        try
        {
            // skip comments
            if (regex_match(strLine, matcher, regexComment))
            {
                logInfo("matched comment: " + trim(matcher[0]));
                continue; // for inline comments, instead of 'continue', just extract the comment and continue processing
            }

            // match section
            if (regex_match(strLine, matcher, regexSection))
            {
                logInfo("matched section: " + trim(matcher[0]));
                handleSection(trim(matcher[0]));
                continue;
            }

            // match key value assigment
            if (regex_match(strLine, matcher, regexKeyValueAssigment))
            {
                logInfo("matched key value assigment: " + trim(matcher[0]));
                handleKeyValueAssigment(trim(matcher[0]));
                continue;
            }
        }
        catch (const regex_error &ex) {
            logError("regex exceptrion");
            return -1;
        }

        if (bSkipInvalidLines) {
            logInfo("matched invalid line, skipping: " + strLine);
        }
        else {
            logError("matched invalid line, skipping: " + strLine);
            throw invalid_format_exception("matched invalid line: " + strLine);
        }
    }

    ifs.close();
    logInfo("done reading " + strFileName);

    // display the internal representation of the parser
    logValues();

    return 0;
}

void IniParser::reset()
{
    values.clear();
    strCurrentSection = "";
}

size_t IniParser::getNoOfValues() const {
    return values.size();
}

const string &IniParser::getValue(const string &strKey)
{
    return getValue("", strKey);
}

const string &IniParser::getValue(const string &strSection, const string &strKey)
{

    string findKey = strKey;

    if (!strSection.empty())
        findKey = strSection + '.' + findKey;

    map<string, string>::const_iterator it = values.find(findKey);
    if (it == values.end())
        throw IniParser::no_such_key_exception();

    return it->second;
}

// throws no_such_key_exception
string IniParser::getString(const string &strSection, const string &strKey)
{
    return getValue(strSection, strKey);
}

// throws no_such_key_exception, invalid_format_exception
int IniParser::getInt(const string &strSection, const string &strKey)
{

    string strValue = getValue(strSection, strKey);
    stringstream stream(strValue);

    int integer;
    if (!(stream >> integer))
        throw IniParser::invalid_format_exception("The value assigned to " + strKey + " is not an integer!");

    return integer;
}

bool IniParser::getBool(const string &strSection, const string &strKey)
{
    string strValue = getValue(strSection, strKey);

    if (strValue == "true")
        return true;
    
    if (strValue == "false")
        return false;

    throw IniParser::invalid_format_exception("The value assigned to " + strKey + " is not a boolean!");    
}

void IniParser::handleSection(const string &strSection)
{
    // even if the regex guarantees that the match is longer than 2, it's better to assert for debuggig purpuses
    assert(strSection.length() >= 2);

    // remove the [ ] and trim once again
    strCurrentSection = trim(strSection.substr(1, strSection.length() - 2));
}

void IniParser::handleKeyValueAssigment(const string &strKeyValueAssigment)
{

    unsigned int pos = strKeyValueAssigment.find('=');

    // TODO: assert here

    string key = trim(strKeyValueAssigment.substr(0, pos - 1));
    string value = trim(strKeyValueAssigment.substr(pos + 1, strKeyValueAssigment.length()));

    if (!strCurrentSection.empty())
        key = strCurrentSection + '.' + key;

    // add or overwrite
    values[key] = value;
}

string IniParser::trim(const string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return string(it, rit.base());
}

void IniParser::logValues()
{
#ifdef DEBUG
    clog << values.size() << " values:\n";
    for (auto it = values.begin(); it != values.end(); ++it)
        clog << it->first << " = " << it->second << '\n';
#endif
}

void IniParser::logInfo(const string &strMsg)
{
#ifdef DEBUG
    clog << "Parser - Info: " + strMsg << endl;
#endif
}

void IniParser::logError(const string &strError)
{
    cerr << "Parser- Error: " + strError << endl;
}
