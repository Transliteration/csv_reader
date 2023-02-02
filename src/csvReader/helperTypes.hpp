#pragma once
#include <string>
#include <variant>

typedef std::variant<std::string, int> Cell;

enum class OpType
{
    value,
    cellIndex,
    error
};

struct Operand
{
    size_t row;
    std::string_view col;
    int value;
    OpType type;
};

struct Operands
{
    Operand lhs, rhs;
};

struct CellIndex
{
    size_t row, col;
};

struct UnknownValueIndex
{
    CellIndex index;
    Operands ops;
    Cell cell;
    char op;
    bool hasError;

    UnknownValueIndex(CellIndex index, Cell &cell);
    UnknownValueIndex(size_t x, size_t y, Cell &cell);
};

std::ostream &operator<<(std::ostream &os, Cell const &va);