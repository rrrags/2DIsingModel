#include <QTimer>
#include "lattice.hpp"

GridModel::GridModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_grid{new Grid{this}}
{
    connect(m_grid, &Grid::gridChanged, this, &GridModel::onGridChanged);
    connect(m_grid, &Grid::newGrid, this, &GridModel::onNewGrid);
    m_timer = new QTimer(this);
}

int GridModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_cells.count();
}

QVariant GridModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_cells.count())
        return QVariant();

    const Grid::State &cell = m_cells.at(index.row());

    switch (role) {
    case Cell:
        return static_cast<int>(cell);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> GridModel::roleNames() const
{
    return {{Cell, "cell"}};
}

void GridModel::setGridSize(int N)
{
    m_timer->stop();
    m_grid->setGridSize(static_cast<std::size_t>(N));
}

void GridModel::makeCellAlive(const int index)
{
    const auto [row, column] = vectorIndexToGridIndex(index);
    m_grid->makeCellAlive(row, column);
}

void GridModel::makeCellDead(const int index)
{
    const auto [row, column] = vectorIndexToGridIndex(index);
    m_grid->makeCellDead(row, column);
}

void GridModel::beginSimulation()
{
    connect(m_timer, &QTimer::timeout, this, [=]() {
        if (m_grid->getNumberOfAliveCells() == 0) {
            m_timer->stop();
        }
        m_grid->nextGeneration();
    });
    m_timer->start(1000);
}

void GridModel::pauseSimulation()
{
    m_timer->stop();
}

void GridModel::onNewGrid(const std::vector<std::vector<Grid::State> > &grid)
{
    beginResetModel();
    m_cells.clear();
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            m_cells.emplace_back(cell);
        }
    }
    endResetModel();
}

void GridModel::onGridChanged(const std::vector<std::pair<size_t, size_t> > &cells)
{
    for (const auto &[row, column] : cells) {
        int indexValue = gridIndexToVectorIndex(row, column);
        m_cells[indexValue] = m_cells[indexValue] == Grid::State::ALIVE ? Grid::State::DEAD
                                                                        : Grid::State::ALIVE;
        QModelIndex idx = index(indexValue);
        emit dataChanged(idx, idx, {Cell});
    }
}

std::size_t GridModel::gridIndexToVectorIndex(size_t row, size_t column) const
{
    return m_grid->gridSize() * row + column;
}

std::pair<std::size_t, std::size_t> GridModel::vectorIndexToGridIndex(size_t index) const
{
    const std::size_t row = std::floor(index / m_grid->gridSize());
    const std::size_t column = index % m_grid->gridSize();
    return std::pair(row, column);
}
