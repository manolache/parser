#include <iostream>
#include <regex>
#include <string>

using namespace std;

string trim(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return string(it, rit.base());
}

int main () {
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