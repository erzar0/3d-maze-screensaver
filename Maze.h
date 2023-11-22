#pragma once
#include "pch.h"
#include "CellGrid.h"
#include "Direction.h"

class Maze {
public:
    Maze(std::size_t width, std::size_t height);

    void generate();

    void draw() const;

    void printMovementPattern() const;

    size_t getSizeX() const;

    size_t getSizeY() const;

    const Cell& getCellAt(int i, int j) const;

    const Direction getMovementDirection(int i) const;

    const std::vector<Direction>& getMovementPattern() const;
private:
    std::vector<std::pair<std::size_t, std::size_t>> getUnvisitedNeighbourPositions(std::size_t i, std::size_t j);

    Direction getDirection(size_t aX, size_t aY, size_t bX, size_t bY);

    CellGrid m_cellGrid;
    std::vector<Direction> m_movementPattern;
    std::mt19937 m_randomGenerator;
};

