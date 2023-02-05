#include "argParser/argParser.hpp"
#include "csvReader/csvReader.hpp"

int main(int argc, char const *argv[])
{
    const ArgParser argParser(argc, argv);
    if (!argParser.parsed_properly())
    {
        return -1;
    }

    CSVReader reader(argParser.get_filepath());

    reader.calc_all_formulas();
    reader.print_table();

    return reader.errorFound;
}
