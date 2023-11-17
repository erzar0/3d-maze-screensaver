#pragma once
#include "pch.h"
#include "CellGrid.h"
#include "Direction.h"

class Maze {
public:
    Maze(std::size_t width, std::size_t height);

    void generate();

    void draw() const;

    size_t getSizeX() const;

    size_t getSizeY() const;

    const Cell& getCellAt(int i, int j) const;
private:
    std::vector<std::pair<std::size_t, std::size_t>> getUnvisitedNeighbourPositions(std::size_t i, std::size_t j);

    Direction getDirection(size_t aX, size_t aY, size_t bX, size_t bY);

    CellGrid m_cellGrid;
    std::mt19937 m_randomGenerator;
};

