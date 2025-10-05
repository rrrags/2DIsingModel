#pragma once

#include <qdebug.h>

#include <QObject>
#include <random>
#include <vector>

/*Convoy's game og life (periodic boundary condition)
-----------------------------------------------------
There are four rules for each cell in this game
    1) Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    2) Any live cell with two or three live neighbours lives on to the next generation.
    3) Any liveh cell with more than three live neighbours dies, as if by overpopulation.
    4) Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

class Simulator
{
  public:
    Simulator() = default;

    enum Spin
    {
        DOWN = -1,
        UP   = 1
    };

    int getlatticeSize() const { return m_latticeSize; };
    void initializeSimulator(int a_latticeSize,
                             bool a_randomSpins,
                             double a_temperature,
                             double a_J);
    std::vector<std::vector<Spin>> getLattice() const;

    void runSimulation();
    void pauseSimulation();
    bool isRunning() const { return m_isRunning; };

    static std::pair<int, int> vectorIndexToLatticeIndex(int a_index, int a_latticeSize);
    static int latticeIndexToVectorIndex(int a_row, int a_column, int a_latticeSize);

  private:
    std::vector<std::vector<Spin>> m_lattice{};
    bool m_isRunning{false};
    int m_latticeSize{};
    double m_beta;  // temperature
    double m_J;     // coupling constant
    const QHash<int, double> probabilityMapper{
        {4, exp(-8.0 * m_beta * m_J)}, {-4, 1.0}, {2, exp(-4.0 * m_beta * m_J)}, {-2, 1.0}, {0, 1.0}};

    double getFlipProbability(int i, int j) const;

    class ProbabilityHandler
    {
      public:
          ProbabilityHandler()
              : generator(rd())
              , realDistribution(0.0, 1.0)
          {}

          void setUpperBoundOnLatticePicker(int latticeSize)
          {
              latticePicker = std::uniform_int_distribution<>(0, latticeSize - 1);
          }
          std::pair<int, int> getRandomLatticeSite(int a_latticeSize)
          {
              return Simulator::vectorIndexToLatticeIndex(latticePicker(generator), a_latticeSize);
        }
        double generateProbability() { return realDistribution(generator); }

      private:
        std::random_device rd{};
        std::mt19937 generator{};
        std::uniform_int_distribution<> latticePicker{};
        std::uniform_real_distribution<> realDistribution{};
    };

    ProbabilityHandler m_probabilityHandler;
};
