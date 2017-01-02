Tested with:
- case sensitive
- UTF-8 
- ubuntu, rasbian 
- gnu compiler 5 and newer
- x86, x86_64 and ARM

Features
- supports keys without sections
- merges duplicate sections
- overwrites keys
- skips invalid lines
- supports unset variables, returns empty string
- throws exceptions

Limitations:
- comments on separates lines only


How to:
make dirs 			- creates a project directory structure
make dirs-test 		- creates a test project directory structure
make deps           - generate dependency files using c++ preprocessor
make deps-test      - generate dependency files for test project using c++ preprocessor
make build          - builds the application
make build-test     - builds the test application
make test           - runs the unit tests
make clean          - clears objects, deps, exe, libs
make all            - builds both app and tests and runs unit tests
make install        - install application
make uninstall      - uninstall application
