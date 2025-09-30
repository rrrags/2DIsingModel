#pragma once

#include <QAbstractListModel>

#include "grid.h"
#include <utility>

class GridModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Grid *grid MEMBER m_grid CONSTANT)
  public:
    enum Roles
    {
        Cell = Qt::UserRole + 1
    };

    explicit GridModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setGridSize(int N);
    Q_INVOKABLE void makeCellAlive(const int index);
    Q_INVOKABLE void makeCellDead(const int index);
    Q_INVOKABLE void beginSimulation();
    Q_INVOKABLE void pauseSimulation();

public slots:
    void onNewGrid(const std::vector<std::vector<Grid::State>> &grid);
    void onGridChanged(const std::vector<std::pair<std::size_t, std::size_t>> &cells);

private:
    QList<Grid::State> m_cells;
    Grid *m_grid{nullptr};
    QTimer *m_timer{nullptr};

    std::pair<std::size_t, std::size_t> vectorIndexToGridIndex(std::size_t index) const;
    std::size_t gridIndexToVectorIndex(std::size_t row, std::size_t column) const;
};
