#include <iostream>
#include <vector>
#include <sstream>
#include <charconv>
#include <iomanip>

#include "util.hpp"

namespace util
{
    int calculate(int lhs, char op, int rhs)
    {
        switch (op)
        {
        case '+':
            return lhs + rhs;
            break;
        case '-':
            return lhs - rhs;
            break;
        case '*':
            return lhs * rhs;
            break;
        case '/':
            return lhs / rhs;
            break;

        default:
            break;
        }

        // unreachable
        throw("");
        return 0;
    }

    // lazy solution, string copy construction involved in sstream constructor
    std::vector<std::string> split_string(const std::string &strToSplit, char delim)
    {
        std::vector<std::string> result;
        std::stringstream ss(strToSplit);
        std::string item;

        while (std::getline(ss, item, delim))
        {
            result.push_back(std::move(item));
        }

        return result;
    }

    Operand parse_operand(std::string_view opStr, std::string_view formula)
    {
        Operand op;
        auto firstNumPos = opStr.find_first_not_of("0123456789");

        // operand is formula
        if (firstNumPos == 0)
        {
            auto posOfFirstLastChar = opStr.find_last_not_of("0123456789");

            auto colName = opStr.substr(0, posOfFirstLastChar + 1);
            op.col = colName;

            auto [ptr, ec]{std::from_chars(opStr.data() + colName.size(), opStr.data() + opStr.size(), op.row)};

            op.type = OpType::cellIndex;

            if (ec == std::errc::invalid_argument)
            {
                std::cerr << "Operand " << std::quoted(opStr) << " in formula " << std::quoted(formula) << " doesn't have a row index" << std::endl;
                op.type = OpType::error;
            }
            else if (ec == std::errc::result_out_of_range)
            {
                std::cerr << "Operand " << std::quoted(opStr) << " in formula " << std::quoted(formula) << " have a row index larger than an int" << std::endl;
                op.type = OpType::error;
            }
        }
        // operand is value
        else
        {
            auto [ptr, ec]{std::from_chars(opStr.data(), opStr.data() + opStr.size(), op.value)};

            op.type = OpType::value;

            if (ec == std::errc::invalid_argument)
            {
                std::cerr << "Operand " << std::quoted(opStr) << " in formula " << std::quoted(formula) << " is neither cell reference nor number" << std::endl;
                op.type = OpType::error;
            }
            else if (ec == std::errc::result_out_of_range)
            {
                std::cerr << "Operand " << std::quoted(opStr) << " in formula " << std::quoted(formula) << " is an value larger than an int" << std::endl;
                op.type = OpType::error;
            }
        }

        return op;
    }

    Operands parse_formula(std::string_view formula, char delim)
    {
        Operands ops;
        size_t delimPos = formula.find(delim);
        ops.lhs = parse_operand(formula.substr(1, delimPos - 1), formula);
        ops.rhs = parse_operand(formula.substr(delimPos + 1), formula);

        return ops;
    }

    // checks for equal sign, operator, and operands
    // (as this function is called while reading file, it cannot tell if operands are correct, but will tell if there is no operands)
    bool is_formula_correct(std::string_view str)
    {
        // = sign check
        auto firstEqualSignPos = str.find('=');
        {
            auto lastEqualSignPos = str.rfind('=');

            if (firstEqualSignPos != lastEqualSignPos)
            {
                std::cerr << "Formula " << std::quoted(str) << " has more then one '=' signs" << std::endl;
                return false;
            }

            if (firstEqualSignPos == std::string::npos)
            {
                std::cerr << "There is no '=' sign in formula " << std::quoted(str) << std::endl;
                return false;
            }

            if (firstEqualSignPos != 0)
            {
                std::cerr << "The formula " << std::quoted(str) << " doesn't start with '=' sign" << std::endl;
                return false;
            }
        }

        // Op sign check
        auto firstOpSignPos = str.find_first_of("+-*/");
        {
            auto lastOpSignPos = str.find_last_of("+-*/");

            if (firstOpSignPos != lastOpSignPos)
            {
                std::cerr << "Formula " << std::quoted(str) << " has more then one of OP signs" << std::endl;
                return false;
            }

            if (firstOpSignPos == std::string::npos)
            {
                std::cerr << "There is no OP sign in formula " << std::quoted(str) << std::endl;
                return false;
            }
        }

        // check if operands exists
        {
            if (firstEqualSignPos + 1 == firstOpSignPos)
            {
                std::cerr << "There is no operand between '=' sign and OP sign in formula " << std::quoted(str) << std::endl;
                return false;
            }

            if (firstOpSignPos == str.size() - 1)
            {
                std::cerr << "There is no operand after " << str[firstOpSignPos] << " sign in formula " << std::quoted(str) << std::endl;
                return false;
            }
        }

        return true;
    }
} // namespace util