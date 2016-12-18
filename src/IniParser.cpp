#include <fstream>

#include "IniParser.h"

IniParser::IniParser() {
    regexComment = regex("(\\s*?)(;|#)(.*)", regex::ECMAScript);
    regexSection = regex("(\\s*?)\\[[^\\]\\r\\n]+](\\s*?)", regex::ECMAScript);
    regexKeyValueAssigment = regex("(\\s*?)(_*[a-z|A-Z][a-z|A-Z|0-9|_]*)([\\s|\\t]*?)=([^;\\r\\n]*)", regex::ECMAScript);
}

IniParser::IniParser(const string& strFileName) {
    IniParser();
}

void IniParser::updateFromFile(const string& strFileName) {
    std::ifstream ifs(strFileName, std::ifstream::in);

}

const string& IniParser::getValue(const string& strKey){
    return strKey;
}


void IniParser::handleKeyValueAssigment(const string& strKeyValueAssigment) {

}


