#include <iostream>

#include "IniParserTestSuite.h"

using namespace std;

int main(int argc, char* args[]) { 
    if (argc != 4) {
        cout << "Invalid arguments! Need at least 3 file paths, empty, first, update, in this particular order\n";
        return -1;
    }

    IniParserTestSuite testSuite(string(args[1], strlen(args[1])), 
                                 string(args[2], strlen(args[2])), 
                                 string(args[3], strlen(args[3])));
    if (!testSuite.runTests()) {
        cout << "One or more tests failed\n";
        return -1;
    } 
    
    cout << "Looks good\n";
    return 0;
}