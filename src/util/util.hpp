#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "../csvReader/helperTypes.hpp"

namespace util
{
    // doesn't check for zero division
    int calculate(int lhs, char op, int rhs);

    // lazy solution, string copy construction involved in sstream constructor
    std::vector<std::string> split_string(const std::string &s, char delim);

    Operand parse_operand(std::string_view opStr, std::string_view formula);

    Operands parse_formula(std::string_view formula, char delim);

    // checks for equal sign, operator, and operands
    // (as this function is called while reading file, it cannot tell if operands are correct, but will tell if there is no operands)
    bool is_formula_correct(std::string_view str);

} // namespace util
