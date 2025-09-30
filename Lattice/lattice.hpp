#pragma once

#include <QObject>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <qdebug.h>
#include <vector>

/*Convoy's game og life (periodic boundary condition)
-----------------------------------------------------
There are four rules for each cell in this game
    1) Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    2) Any live cell with two or three live neighbours lives on to the next generation.
    3) Any liveh cell with more than three live neighbours dies, as if by overpopulation.
    4) Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

class Grid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int gridSize READ gridSize NOTIFY gridSizeChanged)

public:
    Grid(QObject* parent = nullptr) {}

    enum State { DEAD = 0, ALIVE };
    Q_ENUM(State)

    int gridSize() const { return m_gridSize; };
    void setGridSize(const std::size_t gridSize)
    {
        m_gridSize = gridSize;
        m_newGrid.clear();
        m_newGrid.resize(m_gridSize);
        for (auto& row : m_newGrid) {
            row.resize(m_gridSize, State::DEAD);
        }
        m_oldGrid = m_newGrid;
        emit newGrid(m_newGrid);
        emit gridSizeChanged();
    }
    void makeCellAlive(const std::size_t row, const std::size_t column)
    {
        m_newGrid[row][column] = State::ALIVE;
        emit gridChanged(getChangedCells());
        m_oldGrid[row][column] = State::ALIVE;
    }
    void makeCellDead(const std::size_t row, const std::size_t column)
    {
        m_newGrid[row][column] = State::DEAD;
        emit gridChanged(getChangedCells());
        m_oldGrid[row][column] = State::DEAD;
    }

    std::size_t getNumberOfAliveCells() const
    {
        std::size_t count{0};
        for (const auto& row : m_newGrid) {
            count += std::count(row.begin(), row.end(), ALIVE);
        }
        return count;
    }

    void nextGeneration()
    {
        for (std::size_t i = 0; i < m_gridSize; ++i) {
            for (std::size_t j = 0; j < m_gridSize; ++j) {
                std::size_t aliveNeighbours = getNumberOfAliveNeighbours(i, j);
                applyRulesToOldGrid(aliveNeighbours, i, j);
            }
        }
        emit gridChanged(getChangedCells()); // OBS. This must be before: m_newGrid = m_oldGrid;
        m_newGrid = m_oldGrid;
    }

signals:
    void newGrid(const std::vector<std::vector<State>>& grid);
    void gridChanged(const std::vector<std::pair<std::size_t, std::size_t>>& cells);
    void gridSizeChanged();

private:
    std::vector<std::vector<State>> m_newGrid{};
    std::vector<std::vector<State>> m_oldGrid{};
    std::size_t m_gridSize{};

    std::size_t getNumberOfAliveNeighbours(std::size_t i, std::size_t j) const
    {
        // modulus operator
        auto modN = [this](int dividend) {
            return (dividend % static_cast<int>(m_gridSize) + m_gridSize) % m_gridSize;
        };

        // periodic boundary
        std::size_t aliveNeighbours{0};
        if (m_newGrid.at(modN(i + 1)).at(j) == State::ALIVE)
            aliveNeighbours += 1; // lower neighbour
        if (m_newGrid.at(modN(i - 1)).at(j) == State::ALIVE)
            aliveNeighbours += 1; // upper neighbour
        if (m_newGrid.at(i).at(modN(j + 1)) == State::ALIVE)
            aliveNeighbours += 1; // right neighbour
        if (m_newGrid.at(i).at(modN(j - 1)) == State::ALIVE)
            aliveNeighbours += 1; // left neighbour
        if (m_newGrid.at(modN(i - 1)).at(modN(j + 1)) == State::ALIVE)
            aliveNeighbours += 1; // upper right neighbour
        if (m_newGrid.at(modN(i + 1)).at(modN(j + 1)) == State::ALIVE)
            aliveNeighbours += 1; // lower right neighbour
        if (m_newGrid.at(modN(i - 1)).at(modN(j - 1)) == State::ALIVE)
            aliveNeighbours += 1; // upper left neighbour
        if (m_newGrid.at(modN(i + 1)).at(modN(j - 1)) == State::ALIVE)
            aliveNeighbours += 1; // lower left neighbour
        return aliveNeighbours;
    }

    void applyRulesToOldGrid(std::size_t aliveNeighbours, std::size_t i, std::size_t j)
    {
        if (m_newGrid.at(i).at(j) == State::ALIVE) {
            if (aliveNeighbours < 2 || aliveNeighbours > 3)
                m_oldGrid.at(i).at(j) = State::DEAD; // rule 1,2 and 3
        } else {
            if (aliveNeighbours == 3)
                m_oldGrid.at(i).at(j) = State::ALIVE; // rule 4
        }
    }

    const std::vector<std::pair<std::size_t, std::size_t>> getChangedCells() const
    {
        std::vector<std::pair<std::size_t, std::size_t>> changedCells{};
        for (std::size_t row = 0; row < m_gridSize; ++row) {
            for (std::size_t column = 0; column < m_gridSize; ++column) {
                if (m_newGrid[row][column] != m_oldGrid[row][column]) {
                    changedCells.emplace_back(std::pair(row, column));
                }
            }
        }
        return changedCells;
    }
};
