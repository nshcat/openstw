#pragma once

#include <QObject>
#include <QThread>
#include <QTimer>

#include <simulation.hxx>

/**
 * @brief Object that lives inside a worker thread and runs the simulation code.
 */
class SimulationRunner final : public QObject
{
    Q_OBJECT

private:
    constexpr static int updatePeriodMs = 100;

public:
    SimulationRunner();

public:
    Openstw::Simulation::Simulation* simulation() const;
    void startSimulation();

signals:
    void simulationStarted();

protected slots:
    void threadStarted();
    void timerElapsed();

protected:
    QThread* m_thread{};
    Openstw::Simulation::Simulation* m_simulation{};
    QTimer* m_updateTimer{};
    QElapsedTimer m_elapsedTimer{};
};
