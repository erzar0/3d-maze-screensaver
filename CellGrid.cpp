#include "pch.h"
#include "CellGrid.h"

CellGrid::CellGrid(size_t width, size_t height) : m_width{ width }, m_height{ height } 
{
    m_cells = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height));
}

CellGrid::CellGrid() : CellGrid(10, 10) {}

Cell& CellGrid::at(size_t i, size_t j) 
{
    return m_cells[i][j];
}

const Cell& CellGrid::at(size_t i, size_t j) const
{
    return m_cells[i][j];
}

size_t CellGrid::sizeX() const 
{
    return m_width;
}

size_t CellGrid::sizeY() const 
{
    return m_height;
}

bool CellGrid::isSafePosition(std::size_t i, std::size_t j) const 
{
    if (i < 1 || i >= m_width - 1 || j < 1 || j >= m_height - 1) {
        return false;
    }
    return true;
}

