#pragma once

#include <string>
#include <map>
#include <regex>

using namespace std;

/*
 * IniParser
 * Allows users to parse text files formatted as INI and access values based on their keys.
 */
class IniParser
{
public: // methods
    /*
     * constructor
     * @param bSkipInvalidLines - specifies if the parser should ignore the invalid lines
     */     
    IniParser(bool bSkipInvalidLines = false);
    
    /*
     * constructor
     * @param strFileName - specifies the path to the ini file
     * @param bSkipInvalidLines - specifies if the parser should ignore the invalid lines
     */         
    IniParser(const string& strFileName, bool bSkipInvalidLines = false);

    /*
     * copy constructor
     */         
	IniParser(const IniParser& other);
	
    /*
     * delete move constructor
     */         
	IniParser(IniParser&& other) = delete;

    /*
     * copy assignment operator
     */         
	IniParser& operator=(const IniParser& other);
	
    /*
     * delete move assignment operator
     */         
	IniParser& operator=(IniParser&& other) = delete;
	
    /*
     * destructor - so far, this is just a placeholder
     */         
    ~IniParser() {};

    /*
     * updates the internal representation by appending the values from the ini file
     * @param strFileName - specifies the path to the ini file
     * @throws invalid_argument - if the path is invalid
	 *                            it won't change the object - strong guarantee
     * @throws invalid_format_exception - if the parser matches an invalid line
	 *                                    it leaves the object in consistent state - basic guarantee
     * @throws runtime_error - if the parser cannot load values anymore due to memory or some other limitations
	 *                         it leaves the object in consistent state - basic guarantee
	 * @return 0 for success and negative value for error
     */         
    int updateFromFile(const string& strFileName);
    
    /*
     * @return the number of values stored so far
     */         
    size_t size() const;
    
    /*
     * @return the maximum number of values the parser can store
     */         
	size_t max_size() const;
    
    /*
     * clears all the values stored so far
     */
    void clear();
      
    /*
     * gets the value associated to a specific key under a specific section
     * @param strKey - specifies key to look after
     * @param strSection - specifies section to look after
     * @throws invalid_argument - if the key is empty
     * @throws no_such_key_exception - if there is no value for the specified key
     * @throws invalid_format_exception - if the value cannot be casted to the desired type
     * @return the value in generic format
     */
    template <typename T>
    T getValueT(const string& strKey, const string& strSection = "") const {

        stringstream stream(getValue(strKey, strSection));

        T t;
        if (!(stream >> t))
            throw IniParser::invalid_format_exception("The value assigned to " + strKey + " cannot be assigned to the type required!");
        
        return t;
    }

public: // inner classes
    class no_such_key_exception: public runtime_error
    {
    public:
        no_such_key_exception() : runtime_error("No such key exception!") {}
        no_such_key_exception(const string& message) : runtime_error(message) {}
    };

    class invalid_format_exception: public runtime_error
    {
    public:
        invalid_format_exception() : runtime_error("The format is invalid!") {}
        invalid_format_exception(const string& message) : runtime_error(message) {}
    };

private: // methods   

    /*
     * gets the value associated to a specific key under a specific section
     * @param strKey - specifies key to look after
     * @param strSection - specifies section to look after
     * @throws invalid_argument - if the key is empty
     * @throws no_such_key_exception - if there is no value for the specified key
     * @return const reference to the value in string format
     */
    const string& getValue(const string& strKey, const string& section = "") const;

    /*
     * updates the internal representation by adding/updating a key
     * @param strKeyValueAssigment - a string that matches a key value assigment regex
     */
    void handleKeyValueAssigment(const string& strKeyValueAssigment);
    
    /*
     * updates the internal representation by adding/updating a key
     * @param strKeyValueAssigment - a string that matches a key value assigment regex
     */
    void handleSection(const string& strSection);

    /*
     * eliminates the spaces from the beginning and from the ending of a string
     * @param s - a string
     * @return - a new trimmed string
     */
    string trim(const string& s);  

    // logging: tipically, a more robust and configurable logging system is used in production
    // this is just a lazy way to log some text in the standard log/standard error
    
    /*
     * writes the internal representation of the values into the standard log 
     */
    void logValues();
    
    /*
     * writes a formatted log entry into the standard log
     * @param strMsg - the messages that will be formatted and logged
     */
    void logInfo(const string& strMsg);
    
    /*
     * writes a formatted log entry into the standard error
     * @param strMsg - the messages that will be formatted and logged
     */
    void logError(const string& strError);

private: // attributes
    // holds the skip invalid lines mode of operation
    bool m_bSkipInvalidLines;

    // holds the current section while parsing a file
    // it won't span across multiple files.
    string m_strCurrentSection;

	// the internal representatin of an ini file
    // holds the key-value pairs
    map<string, string> m_values;

    // holds a regex that matches the comments into an ini file    
    regex m_regexComment; 

    // holds a regex that matches the sections into an ini file    
    regex m_regexSection;

    // holds a regex that matches the key value assigments into an ini file
    regex m_regexKeyValueAssigment;
};
