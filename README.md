# csv_reader

This app reads the given CSV file, tries to parse the formulas in the cells, and then prints the result to stdout.

If any error occurs during the reading or parsing steps, the error messages will be printed to stderr before the table is printed. The program will also exit with a non-zero code.

To build the executable, run ./build.sh, it will create the YadroTest file in the /bin folder

Test csv files are placed in the test_files folder
To run all tests run ./run_tests.sh

Unit tests code placed in test folder.
To run them type ./bin/util_test

# Or do it all at once
./build.sh && ./run_tests.sh
