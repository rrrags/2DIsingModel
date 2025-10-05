#include "Simulator.hpp"

void Simulator::initializeSimulator(int a_latticeSize,
                                    bool a_randomSpins,
                                    double a_temperature,
                                    double a_J)
{
    m_latticeSize = a_latticeSize;
    m_lattice.clear();
    m_lattice.resize(m_latticeSize);
    for (auto& row : m_lattice) {
        row.resize(m_latticeSize, Spin::UP);
    }
    if (a_randomSpins) {
        for (int i = 0; i < m_latticeSize; ++i) {
            for (int j = 0; j < m_latticeSize; ++j) {
                if (m_probabilityHandler.generateProbability() > 0.5) {
                    m_lattice.at(i).at(j) = Spin::DOWN;
                }
            }
        }
    }
    m_probabilityHandler.setUpperBoundOnLatticePicker(a_latticeSize);
    m_beta = 1.0 / a_temperature;
    m_J = a_J;
}

std::vector<std::vector<Simulator::Spin>> Simulator::getLattice() const
{
    return m_lattice;
}

void Simulator::runSimulation()
{
    m_isRunning = true;
    while (m_isRunning) {
        auto [i, j] = m_probabilityHandler.getRandomLatticeSite(m_latticeSize);
        double flipProbability = getFlipProbability(i, j);
        if (flipProbability > m_probabilityHandler.generateProbability()) {
            m_lattice.at(i).at(j) = m_lattice.at(i).at(j) == Spin::UP ? Spin::DOWN : Spin::UP;
        }
    }
}

void Simulator::pauseSimulation()
{
    m_isRunning = false;
}

std::pair<int, int> Simulator::vectorIndexToLatticeIndex(int a_index, int a_latticeSize)
{
    const int row = std::floor(a_index / a_latticeSize);
    const int column = a_index % a_latticeSize;
    return std::pair(row, column);
}

int Simulator::latticeIndexToVectorIndex(int a_row, int a_column, int a_latticeSize)
{
    return a_latticeSize * a_row + a_column;
}

double Simulator::getFlipProbability(int i, int j) const
{
    // modulus operator
    auto modN = [this](int dividend) {
        return (dividend % m_latticeSize + m_latticeSize) % m_latticeSize;
    };

    // periodic boundary
    int mappingNumber{0};
    mappingNumber += m_lattice.at(modN(i + 1)).at(j); // lower neighbour
    mappingNumber += m_lattice.at(modN(i - 1)).at(j); // upper neighbour
    mappingNumber += m_lattice.at(modN(j + 1)).at(j); // right neighbour
    mappingNumber += m_lattice.at(modN(j - 1)).at(j); // left neighbour
    mappingNumber *= m_lattice.at(i).at(j);

    return probabilityMapper[mappingNumber];
}
