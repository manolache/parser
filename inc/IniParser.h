#include <string>
#include <map>
#include <regex>

using namespace std;

class IniParser
{
public: // methods
    IniParser(bool skipInvalidLines = false);
    IniParser(const string& strFileName, bool skipInvalidLines = false);
    ~IniParser() {};

    void reset();
    int updateFromFile(const string& strFileName);
    
    const string& getValue(const string& section, const string& strKey);
    const string& getValue(const string& strKey);

    int getInt(const string& strSection, const string& strKey);
    int getInt(const string& strKey);

    string getString(const string& strSection, const string& strKey);
    string getString(const string& strKey);
    
    bool getBool(const string& strSection, const string& strKey);
    bool getBool(const string& strKey);

public: // inner classes
    class no_such_key_exception: public logic_error
    {
    public:
        no_such_key_exception(): logic_error("No such key exception!\n"){}
        no_such_key_exception(const string& message): logic_error(message){}
    };

    class invalid_format_exception: public logic_error
    {
    public:
        invalid_format_exception(): logic_error("The format of the ini file is invalid!\n"){}
        invalid_format_exception(const string& message): logic_error(message){}
    };

private: // methods
    void handleKeyValueAssigment(const string& strKeyValueAssigment);
    void handleSection(const string& strSection);

private: // helpers
    string trim(const string& s);
    void logValues();
    void logInfo(const string& strMsg);
    void logError(const string& strError);

private: // attributes
    regex regexComment; 
    regex regexSection;
    regex regexKeyValueAssigment;
    string strCurrentSection;
    map<string, string> values;
    bool bSkipInvalidLines;
};
