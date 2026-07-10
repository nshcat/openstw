#include "simulationrunner.hxx"

SimulationRunner::SimulationRunner() : QObject{}
{
}

Openstw::Simulation::Simulation* SimulationRunner::simulation() const
{
    return this->m_simulation;
}

void SimulationRunner::startSimulation()
{
    this->m_thread = new QThread();
    this->moveToThread(this->m_thread);

    connect(this->m_thread, &QThread::started, this, &SimulationRunner::threadStarted);
    this->m_thread->start();
}

void SimulationRunner::threadStarted()
{
    this->m_simulation = Openstw::Simulation::Simulation::FromTemplateFile(this, "sim_template.xml");

    this->m_elapsedTimer.start();

    this->m_updateTimer = new QTimer(this);
    connect(this->m_updateTimer, &QTimer::timeout, this, &SimulationRunner::timerElapsed);
    this->m_updateTimer->start(SimulationRunner::updatePeriodMs);

    emit this->simulationStarted();
}

void SimulationRunner::timerElapsed()
{
    this->m_simulation->update(static_cast<float>(this->m_elapsedTimer.restart()) / 1000.0f);
}
