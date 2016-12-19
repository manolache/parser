#include <regex>
#include <string>
#include <map>

using namespace std;

class IniParser
{
public: // methods
    IniParser();
    IniParser(const string& strFileName);
    virtual ~IniParser() {};

    int updateFromFile(const string& strFileName);
    
    void reset();
    
    const string& getValue(const string& section, const string& strKey);
    const string& getValue(const string& strKey);

private: // methods
  
    void handleSection ( const string& section );

    string trim(const string& s);

    void handleKeyValueAssigment(const string& strKeyValueAssigment);

private: // attributes
    regex regexComment; 
    regex regexSection;
    regex regexKeyValueAssigment;
    string strCurrentSection;
    map<string, string> values;
};
