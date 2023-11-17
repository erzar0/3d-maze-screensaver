#include "pch.h"
#include "Cell.h"

std::ostream& operator<<(std::ostream& os, const Cell& cell) {
    if (cell.type == CellType::WALL) {
        os << "@";
    }
    else {
        os << " ";
    }
    return os;
}

