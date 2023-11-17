#pragma once
#include "pch.h"
#include "CellType.h"

struct Cell {
    CellType type{ CellType::WALL };
    bool visited{ false };
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);
