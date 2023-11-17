#pragma once
#include "pch.h"
#include "Cell.h"

class CellGrid {
public:
    CellGrid(size_t width, size_t height);

    CellGrid();

    Cell& at(size_t i, size_t j);

    const Cell& at(size_t i, size_t j) const;

    size_t sizeX() const;

    size_t sizeY() const;

    bool isSafePosition(std::size_t i, std::size_t j) const;

private:
    std::size_t m_width{};
    std::size_t m_height{};
    std::vector<std::vector<Cell>> m_cells;
};

