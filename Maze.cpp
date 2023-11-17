#include "pch.h"
#include "assert.h"
#include "Maze.h"

Maze::Maze(std::size_t width, std::size_t height) {
    if (width % 2 == 0) width = width - 1;
    if (height % 2 == 0) height = height - 1;
    m_cellGrid = CellGrid(width, height);

    for (std::size_t j = 1; j < height; j = j + 2) {
        for (std::size_t i = 1; i < width; i = i + 2) {
            if (m_cellGrid.isSafePosition(i, j)) {
                m_cellGrid.at(i, j).type = CellType::PATH;
            }
        }
    }

    std::random_device randomDevice;
    m_randomGenerator = std::mt19937(randomDevice());
}

void Maze::generate() {
    std::stack<std::pair<size_t, size_t>> stack;
    stack.push(std::make_pair(1, 1));
    m_cellGrid.at(1, 1).visited = true;

    while (!stack.empty()) {
        auto const& currentPosition = stack.top();
        auto unvisitedNeighbourPositions = getUnvisitedNeighbourPositions(currentPosition.first, currentPosition.second);

        if (unvisitedNeighbourPositions.size() > 0) {
            std::shuffle(unvisitedNeighbourPositions.begin(), unvisitedNeighbourPositions.end(), m_randomGenerator);
            auto randomPosition = unvisitedNeighbourPositions[0];

            auto& randomCell = m_cellGrid.at(randomPosition.first, randomPosition.second);
            randomCell.visited = true;

            auto direction = getDirection(currentPosition.first, currentPosition.second, randomPosition.first, randomPosition.second);
            switch (direction) {
            case Direction::NORTH:
                m_cellGrid.at(currentPosition.first, currentPosition.second - 1).type = CellType::PATH;
                break;
            case Direction::EAST:
                m_cellGrid.at(currentPosition.first + 1, currentPosition.second).type = CellType::PATH;
                break;
            case Direction::SOUTH:
                m_cellGrid.at(currentPosition.first, currentPosition.second + 1).type = CellType::PATH;
                break;
            case Direction::WEST:
                m_cellGrid.at(currentPosition.first - 1, currentPosition.second).type = CellType::PATH;
                break;
            }

            stack.push(randomPosition);
        }
        else {
            stack.pop();
        }
    }
}

void Maze::draw() const {
    std::cout << "----------------------------------------------------------------------------\n";
    for (std::size_t i = 0; i < m_cellGrid.sizeX(); i++) {
        for (std::size_t j = 0; j < m_cellGrid.sizeY(); j++) {
            std::cout << m_cellGrid.at(i, j);
        }
        std::cout << "\n";
    }
    std::cout << "----------------------------------------------------------------------------\n";
}

std::vector<std::pair<std::size_t, std::size_t>> Maze::getUnvisitedNeighbourPositions(std::size_t i, std::size_t j) {
    auto unvisitedPositions = std::vector<std::pair<size_t, size_t>>{ std::make_pair(i - 2, j), std::make_pair(i, j - 2),
                                                                    std::make_pair(i + 2, j), std::make_pair(i, j + 2) };
    for (auto it = unvisitedPositions.begin(); it != unvisitedPositions.end();) {
        if (!m_cellGrid.isSafePosition(it->first, it->second) || m_cellGrid.at(it->first, it->second).visited) {
            it = unvisitedPositions.erase(it);
        }
        else {
            it++;
        }
    }
    return unvisitedPositions;
}

Direction Maze::getDirection(size_t aX, size_t aY, size_t bX, size_t bY) {
    if (aX < bX) return Direction::EAST;
    if (aX > bX) return Direction::WEST;
    if (aY < bY) return Direction::SOUTH;
    if (aY > bY) return Direction::NORTH;
    assert((0 == 1) && "One direction should be returned!");
}



size_t Maze::getSizeX() const
{
    return m_cellGrid.sizeX();
}

size_t Maze::getSizeY() const
{
    return m_cellGrid.sizeY();
}

const Cell& Maze::getCellAt(int i, int j) const
{
    return m_cellGrid.at(i, j);
}
