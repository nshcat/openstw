#include "stelltisch.hxx"
#include <chrono>

namespace Openstw::SpDr
{
    using namespace std::chrono_literals;

    Stelltisch::Stelltisch(Interlocking::Interlocking* interlocking, QObject* parent)
        : QObject{parent}, m_interlocking{interlocking}
    {
        // == Setup the timer used to blink all tile apertures in a
        // synchronized way
        this->m_blinkTimer = new QTimer(this);
        this->m_blinkTimer->setTimerType(Qt::TimerType::PreciseTimer);
        this->m_blinkTimer->setInterval(500ms);
        this->m_blinkTimer->setSingleShot(false);
        this->m_blinkTimer->setObjectName("StelltischBlinkTimer");

        // Forward the signal as our own. The timer keeps being an implementation
        // detail.
        connect(this->m_blinkTimer, &QTimer::timeout, this, &Stelltisch::blinkStep);

        // Start the timer.
        // XXXTODO is this the correct location for this?
        this->m_blinkTimer->start();
        // ==
    }
}
