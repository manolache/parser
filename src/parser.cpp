#include <iostream>
#include <regex>
#include <string>


#include "IniParser.h"

using namespace std;

int main () {

    IniParser parser;
    parser.updateFromFile("..//string.in");

    string strComment = "  ; comment";
    std::regex regComment("(\\s*?)(;|#)(.*)", regex::ECMAScript); // posix
    std::regex regSection("(\\s*?)\\[[^\\]\\r\\n]+](\\s*?)", regex::ECMAScript); // posix
    std::regex regKeyValue("(\\s*?)(_*[a-z|A-Z][a-z|A-Z|0-9|_]*)([\\s|\\t]*?)=([^;\\r\\n]*)", regex::ECMAScript); // posix
    
    smatch matcher;
    string strSource = "  _a777_7 \t=\t \t 3aads ds dsttt   "; cout<<"source: "<<strSource<<endl;
    auto b = regex_match(strSource, matcher, regKeyValue);
    if (b) {
        cout << "matched: " << matcher[0] << endl;
    } else {
        cout << "no match!" << endl;
    }
    
    return 0;
}