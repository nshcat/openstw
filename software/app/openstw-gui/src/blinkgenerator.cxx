#include "blinkgenerator.hxx"
#include "logging.hxx"
#include <chrono>

using namespace std::chrono_literals;

BlinkGenerator::BlinkGenerator(QObject* parent)
    : QObject{parent}
{
    this->m_blinkTimer = new QTimer(this);
    this->m_blinkTimer->setTimerType(Qt::TimerType::PreciseTimer);
    this->m_blinkTimer->setInterval(500ms);
    this->m_blinkTimer->setSingleShot(false);
    this->m_blinkTimer->setObjectName("StelltischBlinkTimer");

    // clang-format off
    connect(this->m_blinkTimer, &QTimer::timeout,
            this, &BlinkGenerator::onTimerElapsed);
    // clang-format on
}

BlinkState BlinkGenerator::state() const
{
    return this->m_currentState;
}

bool BlinkGenerator::lampsOn() const
{
    return this->m_currentState == BlinkState::LampOn;
}

bool BlinkGenerator::lampsOff() const
{
    return this->m_currentState == BlinkState::LampOff;
}

void BlinkGenerator::stop()
{
    if (!this->m_blinkTimer->isActive())
    {
        qCWarning(catGui, "Stelltisch blink timer is not running, no need to stop");
        return;
    }

    this->m_blinkTimer->stop();
}

void BlinkGenerator::start()
{
    if (this->m_blinkTimer->isActive())
    {
        qCWarning(catGui, "Stelltisch blink timer is already running, no need to start");
        return;
    }

    this->m_blinkTimer->start();
}

void BlinkGenerator::onTimerElapsed()
{
    // Update our blink state.
    this->m_currentState = this->lampsOff() ? BlinkState::LampOn : BlinkState::LampOff;

    // Tell our listeners about it.
    emit this->blink(this->m_currentState);
}
