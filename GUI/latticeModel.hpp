#pragma once

#include <QAbstractListModel>

#include "simulator.hpp"
#include <utility>

class LatticeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Simulator *lattice MEMBER m_simulator CONSTANT)
public:
    enum Roles { SPIN = Qt::UserRole + 1 };

    explicit LatticeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    void latticeChanged(const std::vector<std::vector<Simulator::Spin>> &lattice);

    Q_INVOKABLE void initializeSimulator(int a_latticeSize,
                                         bool a_randomSpins,
                                         double a_temperature,
                                         double a_J);
    Q_INVOKABLE void beginSimulation();
    Q_INVOKABLE void pauseSimulation();
    Q_INVOKABLE void continueSimulation();

private:
    QList<Simulator::Spin> m_spins;
    Simulator *m_simulator{nullptr};
    QTimer *m_timer{nullptr};
    int m_latticeSize{};
};
