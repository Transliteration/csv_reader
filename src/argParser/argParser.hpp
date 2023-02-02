#pragma once

#include <string>

class ArgParser
{
public:
    ArgParser(int argc, char const *argv[]);

    // true only when there is 2 command line arguments, and second arg is path to file, that exist
    bool parsed_properly() const;
    const std::string &get_filepath() const;

private:
    bool parsedProperly;
    std::string filepath;
};