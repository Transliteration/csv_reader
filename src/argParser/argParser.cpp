#include "argParser.hpp"

#include <filesystem>
#include <iostream>

ArgParser::ArgParser(int argc, char const *argv[])
    : parsedProperly(false)
{
    if (argc != 2)
    {
        std::cout << "Program usage:\n"
                  << argv[0] << " [filename]" << std::endl;
        std::cout << "Where [filename] is relative path to csv table to print out." << std::endl;
    }

    if (std::filesystem::exists(argv[1]))
    {
        filepath = argv[1];
        parsedProperly = true;
    }
    else
    {
        std::cerr << "File " << std::quoted(argv[1]) << " doesn't exist." << std::endl;
    }
}

bool ArgParser::parsed_properly() const
{
    return parsedProperly;
}

const std::string &ArgParser::get_filepath() const
{
    return filepath;
}