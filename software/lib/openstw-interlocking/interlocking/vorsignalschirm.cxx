#include "vorsignalschirm.hxx"
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    VorSignalSchirm::VorSignalSchirm(SignalMast* mast, QObject* parent)
        : SignalSchirm{mast, parent}

    {
    }

    VorSignalBild VorSignalSchirm::signalBild() const
    {
        return this->m_signalBild;
    }

    std::optional<SignalMast*> VorSignalSchirm::hauptSignal() const
    {
        return this->m_hauptSignal;
    }

    bool VorSignalSchirm::isBlanked() const
    {
        return this->m_isBlanked;
    }

    void VorSignalSchirm::associateWith(SignalMast* mainSignal)
    {
        // Verify that the signal mast passed as the main signal
        // associated with this distant signal actually has a main signal..
        // Otherwise we can't derive our own Signalbild!
        if (!mainSignal->hasHauptSignalSchirm())
        {
            throw Core::ArgumentError("Signal mast '{}' passed to VorSignalSchirm has no Hauptsignal to refer to",
                                      mainSignal->identifier());
        }

        // If there already is an active association,
        // make sure to revoke it such that no dangling
        // signal connections remain.
        if (this->m_hauptSignal.has_value())
        {
            this->revokeAssociation();
        }

        // Retrieve the Hauptsignalschirm from given main signal mast.
        this->m_hauptSignal = mainSignal;
        this->m_hauptSignalSchirm = mainSignal->hauptSignalSchirm();

        // Connect to the signal aspect changed signal of the Hauptsignal
        // we are dependant on, in order to derive our aspect from its.
        // clang-format off
        connect(this->m_hauptSignalSchirm.value(), &HauptSignalSchirm::signalBildChanged,
                this, &VorSignalSchirm::hauptSignalBildChanged);
        // clang-format on

        // Perform initial synchronization
        this->deriveSignalBild();
    }

    void VorSignalSchirm::revokeAssociation()
    {
        if (this->m_hauptSignalSchirm.has_value())
        {
            // Clear up event connection.
            // clang-format off
            disconnect( this->m_hauptSignalSchirm.value(), &HauptSignalSchirm::signalBildChanged,
                        this, &VorSignalSchirm::hauptSignalBildChanged);
            // clang-format on

            // Then remove association.
            this->m_hauptSignal = std::nullopt;
            this->m_hauptSignalSchirm = std::nullopt;

            // Update the signal bild.
            this->deriveSignalBild();
        }
    }

    void VorSignalSchirm::setIsBlanked(bool isBlanked)
    {
        if (this->m_isBlanked != isBlanked)
        {
            this->m_isBlanked = true;

            // Recompute Signalbild.
            this->deriveSignalBild();
        }
    }

    void VorSignalSchirm::hauptSignalBildChanged(HauptSignalBild signalBild)
    {
        this->deriveSignalBild();
    }

    void VorSignalSchirm::deriveSignalBild()
    {
        VorSignalBild newAspect{VorSignalBild::HaltErwarten};

        // First, check whether the signal is currently blanked.
        // If it is, we turn off all the signal lamps.
        if (this->isBlanked())
        {
            newAspect = VorSignalBild::Off;
        }
        else if (this->m_hauptSignalSchirm.has_value())
        {
            // Otherwise, if we have a main signal associated with this
            // Vorsignal, derive the aspect from its aspect.
            const auto hauptSignalState = this->m_hauptSignalSchirm.value()->signalBild();

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
        }
        else
        {
            // Otherwise, we just show Halt.
            newAspect = VorSignalBild::HaltErwarten;
        }

        // Apply it if its different, and emit signal.
        if (this->m_signalBild != newAspect)
        {
            this->m_signalBild = newAspect;

            emit this->signalBildChanged(this->m_signalBild);
        }
    }
}
