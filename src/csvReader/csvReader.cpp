#include "csvReader.hpp"
#include <iostream>
#include <charconv>
#include <fstream>
#include <iomanip>

std::ostream &operator<<(std::ostream &os, Cell const &va)
{
    std::visit(
        [&](auto &&arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                os << arg;
            else if constexpr (std::is_same_v<T, std::string>)
                os << arg;
        },
        va);

    return os;
}

std::optional<std::reference_wrapper<Cell>> CSVReader::get_cell(const std::string &columnName, int row)
{
    if (!rowIndices.contains(row) || !columnNames.contains(columnName))
    {
        return std::nullopt;
    }

    int _row = rowIndices.at(row);
    int _col = columnNames.at(columnName);
    return cells[_row][_col];
}

std::optional<int> CSVReader::read_operand(const Operand &op, UnknownValueIndex &context)
{
    switch (op.type)
    {
    case OpType::error:
    {
        std::cerr << "Error in formula "
                  << " cell {" << context.cell << '}' << std::endl;
        std::cerr << "Couldn't read left operand" << std::endl;
        context.hasError = true;
        break;
    }
    case OpType::value:
    {
        return op.value;
        break;
    }
    case OpType::cellIndex:
    {
        auto cell = get_cell(std::string(op.col), op.row);
        if (cell.has_value())
        {
            if (const int *value = std::get_if<int>(&cell.value().get()))
            {
                return *value;
            }
        }
        else
        {
            std::cerr << "Error: there is no cell in table with index {" << op.col << op.row << '}' << std::endl;
            context.hasError = true;
        }
        break;
    }
    default:
        break;
    }

    return std::nullopt;
}

bool CSVReader::parse_op(UnknownValueIndex &index)
{
    const auto &str = std::get<std::string>(index.cell);
    {
        auto opPos = str.find_first_of("+-*/");
        index.op = str[opPos];
    }

    index.ops = util::parse_formula(str, index.op);
    auto [lhs, rhs] = index.ops;

    if (lhs.type == OpType::error || rhs.type == OpType::error)
    {
        index.hasError = true;
        return false;
    }

    std::optional<int> lhsValue = read_operand(lhs, index);
    std::optional<int> rhsValue = read_operand(rhs, index);

    if (index.op == '/' && rhsValue.has_value() && rhsValue == 0)
    {
        std::cerr << "Zero division error in cell " << str << std::endl;
        index.hasError = true;
        return false;
    }

    if (lhsValue.has_value() && rhsValue.has_value())
    {
        Cell &cell = cells[index.index.row][index.index.col];
        int operand1 = lhsValue.value();
        int operand2 = rhsValue.value();

        cell = util::calculate(operand1, index.op, operand2);
        return true;
    }

    return false;
}

void CSVReader::calc_all_formulas()
{
    int resolvedThisLoop = 0;

    do
    {
        resolvedThisLoop = 0;
        std::vector<size_t> resolvedFormulaIndexes; // holds indexes of resolved formulas

        for (size_t index = 0; index < formulasToCalculate.size(); ++index)
        {
            auto &val = formulasToCalculate[index];
            if (val.hasError)
            {
                resolvedFormulaIndexes.push_back(index);
                errorFound = true;
                continue;
            }
            if (parse_op(val))
            {
                ++resolvedThisLoop;
                resolvedFormulaIndexes.push_back(index);
            }
        }

        std::reverse(resolvedFormulaIndexes.begin(), resolvedFormulaIndexes.end());

        for (auto &i : resolvedFormulaIndexes)
        {
            formulasToCalculate.erase(formulasToCalculate.begin() + i);
        }

    } while (resolvedThisLoop);

    for (const auto &cell : formulasToCalculate)
    {
        if (!cell.hasError)
        {
            std::cerr << "A circular dependency (and/or dependency on empty cell) was found in the formula " << cell.cell << ", in cell " << cells[0][cell.index.col] << cells[cell.index.row][0] << std::endl;
        }

        errorFound = true;
    }
}

void CSVReader::print_table() const
{
    for (size_t i = 0; i < cells.size(); i++)
    {
        for (size_t j = 0; j < cells[i].size(); j++)
        {
            std::cout << cells[i][j] << ",";
        }

        std::cout << '\n';
    }
}

bool CSVReader::check_read_line(const std::vector<std::string> &lineTokens, int &rowId, int rowNumber)
{
    {
        const auto tokens_size = lineTokens.size();
        const auto column_size = columnNames.size();

        if (tokens_size != column_size)
        {
            std::cerr << "The number of columns in row number " << rowNumber
                      << " does not match the number of columns in the header"
                      << std::endl;
        }
    }

    auto &rowIdStr = lineTokens[0];
    bool containsOnlyDigits =
        rowIdStr.find_first_not_of("0123456789") == std::string::npos;
    if (!containsOnlyDigits)
    {
        std::cerr << "Row identificator " << std::quoted(rowIdStr) << " in row #" << rowNumber + 1
                  << " have characters other than digits " << std::endl;
        errorFound = true;
        return false;
    }

    auto [ptr, ec]{std::from_chars(rowIdStr.data(), rowIdStr.data() + rowIdStr.size(), rowId)};

    if (ec == std::errc::invalid_argument)
    {
        std::cerr << "Row identificator " << std::quoted(rowIdStr) << " in row #" << rowNumber + 1
                  << " does not consist entirely of digits " << std::endl;
        errorFound = true;
        return false;
    }
    else if (ec == std::errc::result_out_of_range)
    {
        std::cerr << "Row identificator " << std::quoted(rowIdStr) << " in row #" << rowNumber + 1 << " is larger than an int.\n";
        errorFound = true;
        return false;
    }

    if (bool inserted =
            rowIndices.insert_or_assign(rowId, rowNumber).second;
        !inserted)
    {
        std::cerr << "Row id is repeated in row number " << rowNumber + 1
                  << std::endl;
        return false;
    }

    return true;
}

bool CSVReader::read_file(std::string filename)
{
    std::ifstream input(filename);

    std::string headerLine;
    getline(input, headerLine);
    bool wereErrorsFound = read_header(headerLine);

    size_t rowNumber = 1;

    for (std::string line; getline(input, line);)
    {
        auto lineTokens = util::split_string(line, ',');
        int rowId = 0;
        if (!check_read_line(lineTokens, rowId, rowNumber))
        {
            wereErrorsFound = true;
        }

        if (lineTokens.size() != columnNames.size())
        {
            lineTokens.resize(columnNames.size());
        }

        cells.push_back({});
        auto &currentRow = cells[rowNumber];
        currentRow.resize(lineTokens.size());
        currentRow.front() = rowId;

        for (size_t columnNumber = 1; columnNumber < lineTokens.size();
             columnNumber++)
        {
            const auto &cellStr = lineTokens[columnNumber];

            const bool containsOnlyDigits =
                cellStr.find_first_not_of("0123456789") == std::string::npos;

            const bool empty = cellStr.empty();

            if (containsOnlyDigits && !empty)
            {
                int number = 0;
                std::from_chars(cellStr.data(), cellStr.data() + cellStr.size(), number);
                currentRow[columnNumber] = number;
            }
            else if (empty)
            {
                std::cerr << "Cell with name " << cells.front()[columnNumber] << cells[rowNumber].front() << " is empty" << std::endl;
                currentRow[columnNumber] = "";
                wereErrorsFound = true;
            }
            else if (util::is_formula_correct(cellStr))
            {
                currentRow[columnNumber] = cellStr;

                UnknownValueIndex formula;
                formula.cell = currentRow[columnNumber];
                formula.index.col = columnNumber;
                formula.index.row = rowNumber;

                formulasToCalculate.push_back(std::move(formula));
            }
            else
            {
                std::cerr << "Incorrect formula in cell " << cells.front()[columnNumber] << cells[rowNumber].front() << std::endl;
                currentRow[columnNumber] = cellStr;
                wereErrorsFound = true;
            }
        }

        ++rowNumber;
    }

    return wereErrorsFound;
}

bool CSVReader::read_header(const std::string &line)
{
    const auto headerTokens = util::split_string(line, ',');
    bool wereErrorsFound = false;

    cells.resize(1);
    auto &currentRow = cells.front();

    int columnNumber = 0;

    for (const auto &columnName : headerTokens)
    {
        currentRow.push_back(columnName);

        if (bool inserted = columnNames
                                .insert_or_assign(std::move(columnName), columnNumber)
                                .second;
            !inserted)
        {
            std::cerr << "Column names in header are repeated" << std::endl;
            wereErrorsFound = true;
        }

        ++columnNumber;
    }

    return wereErrorsFound;
}

CSVReader::CSVReader(std::string filename) : errorFound(false)
{
    errorFound = read_file(filename);
}
