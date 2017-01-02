#include <iostream>

#include "IniParser.h"

using namespace std;

int main(int argc, char* args[]) {

	IniParser parser(true);

	for (auto i = 1; i < argc; i++) {
		try {
			int nReturn = parser.updateFromFile(args[i]);
			if (nReturn != 0) {
				cout << "internal error:" << nReturn << endl;
				return nReturn;
			}
		} catch (invalid_argument ex) {
			cout << ex.what() << endl;
			return -1; 
		} catch (IniParser::invalid_format_exception ex) {
			cout << ex.what() << endl;
			return -1; 
		} catch (const runtime_error& ex) {
			cout << ex.what() << endl;
			return -1; 
		}
	}
    return 0;
}