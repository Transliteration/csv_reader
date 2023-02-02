#pragma once

#include <variant>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>

#include "../util/util.hpp"
#include "helperTypes.hpp"

class CSVReader
{
private:
    std::vector<std::vector<Cell>> cells;
    std::unordered_map<std::string, int> columnNames; // map column names to indexes in "cells" variable
    std::unordered_map<int, int> rowIndices;          // map row indexes to indexes in "cells" variable
    std::vector<UnknownValueIndex> formulasToCalculate;

public:
    bool errorFound;

    CSVReader(std::string filename);

    // returns reference to Cell, if it exists, nullopt otherwise
    std::optional<std::reference_wrapper<Cell>> get_cell(const std::string &columnName, int rowIndex);

    // return value of cell if cell has it, nullopt otherwise
    std::optional<int> read_operand(const Operand &op, UnknownValueIndex &context);

    // the formula is checked for correctness, but the operands are not
    bool parse_op(UnknownValueIndex &index);

    // trying to resolve all formulas found while reading file
    // iterates throught formulas in loop, until 0 formulas resolved in iteration
    void calc_all_formulas();

    // print whole table, can be called at any time
    void print_table() const;

    bool check_read_line(const std::vector<std::string> &lineTokens, int &rowId, int rowNumber);

    bool read_file(std::string filename);

    bool read_header(const std::string &line);
};
