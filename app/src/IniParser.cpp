#include <iostream>
#include <fstream>
#include <exception>
#include <assert.h>

#include "IniParser.h"

using namespace std;

#define OP_COMMENT1             ';'
#define OP_COMMENT2             '#'
#define OP_ASSIGN               '='
#define OP_SECTION_START        "["
#define OP_SECTION_END          "]"
#define OP_SECTION_KEY_CAT      '.'

#define REG_SPACES              "([\\s|\\t]*)" 

IniParser::IniParser(bool bSkipInvalidLines) {

    m_bSkipInvalidLines = bSkipInvalidLines;

    stringstream stream;

    // comments can either start with ; or #
    stream << REG_SPACES;                                           // spaces
    stream << '(' << OP_COMMENT1 << '|' << OP_COMMENT2 << ")";      // ; or #
    stream << "(.*)";                                               // anything
    m_regexComment = regex(stream.str(), regex::ECMAScript);		

    stream.str("");
    stream.clear();   

    // sections are placed between square brackets:  [Section.SubSection... and so on]
    stream << REG_SPACES;                                           // spaces
    stream << "\\" << OP_SECTION_START;                             // operator start section
    stream << "[^" << "\\" << OP_SECTION_END << "\\r\\n]+";         // anything except ] or line breaks
    stream << OP_SECTION_END;                                       // operator end section
    stream << REG_SPACES;                                           // spaces once again
    m_regexSection = regex(stream.str(), regex::ECMAScript);		
    
    stream.str("");
    stream.clear();
    
	// key value assigment
    stream << REG_SPACES;                                           // spaces
    stream << "(_*[a-z|A-Z][_|a-z|A-Z|0-9]*)";                      // _ repeat letter  digit or letter or _ repeat
    stream << REG_SPACES;
    stream << OP_ASSIGN;                                            // assign operator
    stream << "([^\\r\\n]*)";                                       // anything except line breaks
    m_regexKeyValueAssigment = regex(stream.str(), regex::ECMAScript);
    
    clear();
}

// just delegate the params constructor and process the file
IniParser::IniParser(const string &strFileName, bool bSkipInvalidLines) 
	: IniParser(bSkipInvalidLines) 
{
    updateFromFile(strFileName);
}

IniParser::IniParser(const IniParser& other) {

	m_bSkipInvalidLines 		= other.m_bSkipInvalidLines;
	m_regexComment 				= other.m_regexComment;
	m_regexSection 				= other.m_regexSection;
	m_regexKeyValueAssigment 	= other.m_regexKeyValueAssigment;
	m_values.insert(other.m_values.begin(), other.m_values.end());
}


IniParser& IniParser::operator=(const IniParser& other) {

	m_bSkipInvalidLines 		= other.m_bSkipInvalidLines;
	m_regexComment 				= other.m_regexComment;
	m_regexSection 				= other.m_regexSection;
	m_regexKeyValueAssigment 	= other.m_regexKeyValueAssigment;
	m_values.insert(other.m_values.begin(), other.m_values.end());

	return *this;
}

int IniParser::updateFromFile(const string &strFileName) {

    // check for empty file name
    if (strFileName.empty())
        throw invalid_argument("The input file name is empty!");

    // open file to read from
    ifstream ifs(strFileName, std::ifstream::in);

    // check if file was open
    if (ifs.fail())
        throw invalid_argument("Unable to open the input file " + strFileName);

    // start with an empty section
    m_strCurrentSection = "";

    logInfo("reading " + strFileName);
    while (!ifs.eof()) {
        string strLine;

        // read line
        getline(ifs, strLine);

        //skip empty lines
        if (strLine.empty()) {
            logInfo("matched empty line, skipping...");
            continue;
        }

        smatch matcher;
        try {
            // skip comments
            if (regex_match(strLine, matcher, m_regexComment)) {
                logInfo("matched comment: " + trim(matcher[0]));
                // TODO: for inline comments, instead of 'continue', just extract the comment and continue processing
                continue; 
            }

            // match section
            if (regex_match(strLine, matcher, m_regexSection)) {
                logInfo("matched section: " + trim(matcher[0]));
                handleSection(trim(matcher[0]));
                continue;
            }

            // match key value assigment
            if (regex_match(strLine, matcher, m_regexKeyValueAssigment)) {
                logInfo("matched key value assigment: " + trim(matcher[0]));
                handleKeyValueAssigment(trim(matcher[0]));
                continue;
            }
        } catch (const regex_error &ex) {
            logError("regex exception");
            return -1;
        }

        if (m_bSkipInvalidLines) {
            logInfo("matched invalid line, skipping: " + strLine);
        } else {
            logError("matched invalid line: " + strLine);
            throw invalid_format_exception("matched invalid line: " + strLine);
        }
    }

    ifs.close();
    logInfo("done reading " + strFileName);

    // display the internal representation of the parser
    logValues();

    return 0;
}

size_t IniParser::size() const {
    return m_values.size();
}


size_t IniParser::max_size() const {
    return m_values.max_size();
}

void IniParser::clear() {
    m_values.clear();
    m_strCurrentSection = "";
}

const string &IniParser::getValue(const string &strKey, const string &strSection) const {

    if (strKey.empty())
        throw invalid_argument("The find key is empty!");

    string strFindKey = strKey;

    if (!strSection.empty())
        strFindKey = strSection + OP_SECTION_KEY_CAT + strFindKey;

    map<string, string>::const_iterator it = m_values.find(strFindKey);
    if (it == m_values.end())
        throw IniParser::no_such_key_exception();

    return it->second;
}

string IniParser::getString(const string &strKey, const string &strSection) const {

    return getValue(strKey, strSection);
}

int IniParser::getInt(const string &strKey, const string &strSection) const {

    stringstream stream(getValue(strKey, strSection));

    int integer;
    if (!(stream >> integer))
        throw IniParser::invalid_format_exception("The value assigned to " + strKey + " is not an integer!");

    return integer;
}

bool IniParser::getBool(const string &strKey, const string &strSection) const {

    string strValue = getValue(strKey, strSection);

    if (strValue == "true")
        return true;
    
    if (strValue == "false")
        return false;

    throw IniParser::invalid_format_exception("The value assigned to " + strKey + " is not a boolean!");    
}

void IniParser::handleSection(const string &strSection) {

    // even if the regex guarantees that the match is longer than 2, it's better to assert for debuggig purpuses
    assert(strSection.length() >= 2);

    // remove the [ ] and trim once again
    m_strCurrentSection = trim(strSection.substr(1, strSection.length() - 2));
}

void IniParser::handleKeyValueAssigment(const string &strKeyValueAssigment) {

    unsigned int pos = strKeyValueAssigment.find(OP_ASSIGN);

    assert(0 < pos && pos < strKeyValueAssigment.length());

    string key = trim(strKeyValueAssigment.substr(0, pos - 1));
    string value = trim(strKeyValueAssigment.substr(pos + 1, strKeyValueAssigment.length()));

    if (!m_strCurrentSection.empty())
        key = m_strCurrentSection + OP_SECTION_KEY_CAT + key;

	try	{
    	// insert or overwrite - throws "an exception" if the insert fails
		// the documentation is quite vague and it doesn't say what exception is thrown.
		// catching const reference to exception to prevent slicing
		// throw a runtime error with verbose details, so the user can gracefully handle this
	    m_values[key] = value;
	} catch (const exception& ex) {
		logError(string("Unable to insert values into map") + ex.what());
		throw runtime_error("Unable to load values! Max capacity is " + max_size());
	}
}

string IniParser::trim(const string &s) {

    string::const_iterator lit = s.begin();
    while (lit != s.end() && isspace(*lit))
        lit++;

    string::const_reverse_iterator rit = s.rbegin();
    while (lit != rit.base() && isspace(*rit))
        rit++;

    return string(lit, rit.base());
}

void IniParser::logValues() {

#ifdef DEBUG
    clog << m_values.size() << " values:\n";
    for (auto it = m_values.begin(); it != m_values.end(); ++it)
        clog << it->first << " = " << it->second << '\n';
#endif
}

void IniParser::logInfo(const string &strMsg) {

#ifdef DEBUG
    clog << "Parser - Info: " + strMsg << endl;
#endif
}

void IniParser::logError(const string &strError) {

    cerr << "Parser- Error: " + strError << endl;
}
