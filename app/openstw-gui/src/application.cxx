#include "application.hxx"
#include <stdexcept>

Application* gApplication{nullptr};

Application::Application() : QObject{}
{
    this->startTimers();
}

QTimer* Application::timer1Hz()
{
    return this->m_timer1Hz;
}

QTimer* Application::timer2Hz()
{
    return this->m_timer2Hz;
}

void Application::startTimers()
{
    this->m_timer1Hz = new QTimer(this);
    this->setupTimer(this->m_timer1Hz, 1000);

    this->m_timer2Hz = new QTimer(this);
    this->setupTimer(this->m_timer2Hz, 500);
}

void Application::setupTimer(QTimer* timer, int periodMs)
{
    timer->start(periodMs);
}

void createApplication()
{
    if (gApplication != nullptr)
        throw std::runtime_error("Application object already created");

    gApplication = new Application();
}
