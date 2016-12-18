#include <regex>
#include <string>
#include <map>

using namespace std;

class IniParser
{
public: // methods
    IniParser();
    IniParser(const string& strFileName);

    void updateFromFile(const string& strFileName);
    void reset();
    
    const string& getValue(const string& strKey);

private: // methods
    void handleKeyValueAssigment(const string& strKeyValueAssigment);

private: // attributes
    // comments can either start with ; or #:  ;;;this is a comment
    regex regexComment; 
    // sections are placed between square brackets:  [Section.SubSection... and so on]
    regex regexSection;
    // = is used to assign values to key
    regex regexKeyValueAssigment;
    
    map<string, string> values;
};
