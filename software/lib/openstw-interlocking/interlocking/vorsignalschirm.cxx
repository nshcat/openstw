#include "vorsignalschirm.hxx"
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    VorSignalSchirm::VorSignalSchirm(SignalMast* mast, SignalMast* mainSignal, QObject* parent)
        : SignalSchirm{mast, parent}

    {
        // Verify that the signal mast passed as the main signal
        // associated with this distant signal actually has a main signal..
        // Otherwise we can't derive our own Signalbild!
        if (!mainSignal->hasHauptSignalSchirm())
        {
            throw Core::ArgumentError("Signal mast '{}' passed to VorSignalSchirm has no Hauptsignal to refer to",
                                      mainSignal->identifier());
        }

        // Retrieve the Hauptsignalschirm from given main signal mast.
        this->m_hauptSignal = mainSignal;
        this->m_hauptSignalSchirm = mainSignal->hauptSignalSchirm();

        // Connect to the signal aspect changed signal of the Hauptsignal
        // we are dependant on, in order to derive our aspect from its.
        // clang-format off
        connect(this->m_hauptSignalSchirm, &HauptSignalSchirm::signalBildChanged,
                this, &VorSignalSchirm::deriveSignalBild);
        // clang-format on

        // Perform initial synchronization
        this->deriveSignalBild(this->m_hauptSignalSchirm->signalBild());
    }

    VorSignalBild VorSignalSchirm::signalBild() const
    {
        return this->m_signalBild;
    }

    SignalMast* VorSignalSchirm::hauptSignal() const
    {
        return this->m_hauptSignal;
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
