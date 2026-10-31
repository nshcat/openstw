#include "vorsignalschirm.hxx"

namespace Openstw::Interlocking
{

    VorSignalSchirm::VorSignalSchirm(HauptSignalSchirm* hauptSignal, QObject* parent)
        : QObject{parent}, m_hauptSignal{hauptSignal}

    {
        // Connect to the signal aspect changed signal of the Hauptsignal
        // we are dependant on, in order to derive our aspect from its.
        // clang-format off
        connect(this->m_hauptSignal, &HauptSignalSchirm::signalBildChanged,
                this, &VorSignalSchirm::deriveSignalBild);
        // clang-format on

        // Perform initial synchronization
        this->deriveSignalBild(this->m_hauptSignal->signalBild());
    }

    VorSignalBild VorSignalSchirm::signalBild() const
    {
        return this->m_signalBild;
    }

    void VorSignalSchirm::deriveSignalBild(HauptSignalBild hauptSignalState)
    {
        // Determine new signal aspect based on the main signal aspect.
        VorSignalBild newAspect{VorSignalBild::HaltErwarten};

        switch (hauptSignalState)
        {
        case HauptSignalBild::Halt:
            newAspect = VorSignalBild::HaltErwarten;
            break;

        case HauptSignalBild::Fahrt:
            newAspect = VorSignalBild::FahrtErwarten;
            break;

        case HauptSignalBild::LangsamFahrt:
            newAspect = VorSignalBild::LangsamFahrtErwarten;
            break;

        default:
            break;
        }

        // Apply it if its different, and emit signal.
        if (this->m_signalBild != newAspect)
        {
            this->m_signalBild = newAspect;

            emit this->signalBildChanged(this->m_signalBild);
        }
    }
}
