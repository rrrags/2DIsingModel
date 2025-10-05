#include "latticeModel.hpp"
#include <QTimer>
#include <QtConcurrent>

LatticeModel::LatticeModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_simulator{new Simulator{}}
{
    m_timer = new QTimer(this);
}

int LatticeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_spins.count();
}

QVariant LatticeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_spins.count())
        return QVariant();

    const Simulator::Spin &spin = m_spins.at(index.row());

    switch (role) {
    case SPIN:
        return static_cast<int>(spin);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> LatticeModel::roleNames() const
{
    return {{SPIN, "spin"}};
}

void LatticeModel::initializeSimulator(int a_latticeSize,
                                       bool a_randomSpins,
                                       double a_temperature,
                                       double a_J)
{
    m_simulator->initializeSimulator(a_latticeSize, a_randomSpins, a_temperature, a_J);
    m_latticeSize = a_latticeSize;
}

void LatticeModel::beginSimulation()
{
    QFuture<void> future = QtConcurrent::run([this]() { m_simulator->runSimulation(); });
    connect(m_timer, &QTimer::timeout, this, [this]() {
        if (m_simulator->isRunning()) {
            latticeChanged(m_simulator->getLattice());
        };
    });
    m_timer->start(1000);
}

void LatticeModel::pauseSimulation()
{
    m_simulator->pauseSimulation();
}

void LatticeModel::continueSimulation()
{
    QFuture<void> future = QtConcurrent::run([this]() { m_simulator->runSimulation(); });
}

void LatticeModel::latticeChanged(const std::vector<std::vector<Simulator::Spin>> &a_lattice)
{
    static std::vector<std::vector<Simulator::Spin>> previousLattice{a_lattice};

    for (int i = 0; i < m_latticeSize; ++i) {
        for (int j = 0; j < m_latticeSize; ++j) {
            if (previousLattice.at(i).at(j) != a_lattice.at(i).at(j)) {
                int indexValue = Simulator::latticeIndexToVectorIndex(i, j, m_latticeSize);
                m_spins[indexValue] = m_spins[indexValue] == Simulator::Spin::UP
                                          ? Simulator::Spin::UP
                                          : Simulator::Spin::DOWN;
                QModelIndex idx = index(indexValue);
                emit dataChanged(idx, idx, {SPIN});
            }
        }
    }
}
