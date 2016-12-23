#include <iostream>

#include "IniParserTestSuite.h"

using namespace std;

int main(int argc, char* args[]) { 
    if (argc != 4) {

        cout << "Invalid arguments! Need at least 3 file paths, empty, first, update, in this order\n";
        return -1;
    }

    IniParserTestSuite testSuite(string(args[1], strlen(args[1])), 
                                 string(args[2], strlen(args[2])), 
                                 string(args[3], strlen(args[3])));
    testSuite.runTests();

    return 0;
}