#include <iostream>

#include "IniParser.h"

using namespace std;

int main () {

    try {
        IniParser parser("..//test1.ini", true);
        parser.updateFromFile("..//test1.ini");
        parser.reset();
        parser.updateFromFile("..//test1.ini");
        cout << parser.getValue("section.subsection", "city") << endl;
        cout << parser.getBool("section.subsection", "isNice") << endl;
        cout << parser.getInt("section", "key2") << endl;
        parser.getString("", "mucipeclanta");
    } catch (invalid_argument ex) {
        cout<< ex.what();
        return -1;          // gracefully handle exception        
    } catch (IniParser::invalid_format_exception ex) {
        cout<< ex.what();   // gracefully handle exception
        return -1;
    } catch (IniParser::no_such_key_exception ex) {
        cout<< ex.what();   // gracefully handle exception
        return -1;
    }
    
    return 0;
}