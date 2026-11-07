#include "signalmast.hxx"

#include "../hauptsignalschirm.hxx"
#include "../vorsignalschirm.hxx"
#include "logging.hxx"

#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    SignalMast::SignalMast(QStringView identifier, Topology::Signal* topoNode, QObject* parent)
        : Signal{ElementType::SignalMast,
                 identifier,
                 topoNode,
                 {LockingReason::RouteDestination, LockingReason::FlankProtection, LockingReason::RouteStart},
                 true,
                 parent}
    {
    }

    bool SignalMast::hasHauptSignalSchirm() const
    {
        return this->m_hauptSignal.has_value();
    }

    const HauptSignalSchirm* SignalMast::hauptSignalSchirm() const
    {
        return *this->m_hauptSignal;
    }

    HauptSignalSchirm* SignalMast::hauptSignalSchirm()
    {
        return *this->m_hauptSignal;
    }

    bool SignalMast::hasVorSignalSchirm() const
    {
        return this->m_vorSignal.has_value();
    }

    const VorSignalSchirm* SignalMast::vorSignalSchirm() const
    {
        return *this->m_vorSignal;
    }

    VorSignalSchirm* SignalMast::vorSignalSchirm()
    {
        return *this->m_vorSignal;
    }

    bool SignalMast::isMainSignal() const
    {
        return this->hasHauptSignalSchirm();
    }

    bool SignalMast::isShuntingSignal() const
    {
        // XXX TODO Signalmast could support showing a Sh1.
        return false;
    }

    bool SignalMast::isDistantSignal() const
    {
        return this->hasVorSignalSchirm();
    }

    std::optional<Signal*> SignalMast::associatedSignal() const
    {
        // If we do not have a Vorsignalschirm, we cant have an associated
        // signal.
        if (!this->hasVorSignalSchirm())
        {
            return std::nullopt;
        }

        // Ask the Vorsignalschirm which signal it refers to.
        // It will always be a SignalMast, but those are also Signals.
        // The caller doesnt care about that implementation detail..
        return this->vorSignalSchirm()->hauptSignal();
    }

    bool SignalMast::hasAssociatedSignal() const
    {
        return this->associatedSignal().has_value();
    }

    void SignalMast::associateWithSignal(Signal* mainSignal)
    {
        // If we do not have a Vorsignalschirm, we cant have an associated
        // signal.
        if (!this->hasVorSignalSchirm())
        {
            // clang-format off
            qCCritical(catInterlocking)
                << "Can't set signal association for signal '"
                << this->identifier()
                << "': Signal has no Vorsignalschirm"
                << Qt::endl;
            // clang-format on

            return;
        }

        // We need the other signal to be a SignalMast as well.
        // XXXTODO figure out how to handle interacting with
        // virtual signals! Probably, in those cases, the routing engine
        // will actually pass the first block signal into this method..
        if (mainSignal->type() != ElementType::SignalMast)
        {
            // clang-format off
            qCCritical(catInterlocking)
                << "Can't set signal association for signal '"
                << this->identifier()
                << "': Passed signal '"
                << mainSignal->identifier()
                << "' is not a SignalMast"
                << Qt::endl;
            // clang-format on

            return;
        }

        auto* mainSignalMast = dynamic_cast<SignalMast*>(mainSignal);

        if (!mainSignalMast)
        {
            throw Core::ArgumentError("Main signal '{}' was unexpectedly not a SignalMast", mainSignal->identifier());
        }

        this->vorSignalSchirm()->associateWith(mainSignalMast);
    }

    void SignalMast::revokeSignalAssociation()
    {
        // If we do not have a Vorsignalschirm, we cant have an associated
        // signal.
        if (!this->hasVorSignalSchirm())
        {
            // clang-format off
            qCWarning(catInterlocking)
                << "Can't revoke signal association for signal '"
                << this->identifier()
                << "': Signal has no Vorsignalschirm"
                << Qt::endl;
            // clang-format on

            return;
        }

        this->vorSignalSchirm()->revokeAssociation();
    }

    void SignalMast::onMovementAuthorityChanged()
    {
        // First, we need to validate that we have a Hauptsignalschirm.
        // If not, we can't do anything with the movement authority!
        if (!this->hasHauptSignalSchirm())
        {
            // Log this, because it really shouldnt occur.
            // clang-format off
            qCCritical(catInterlocking)
                << "Signal '"
                << this->identifier()
                << "' can't handle new movement authority: Signal has no Hauptsignalschirm"
                << Qt::endl;
            // clang-format on

            return;
        }

        // Retrieve our Hauptsignalschirm.
        auto* hauptsignal = this->hauptSignalSchirm();

        // Now, check if theres a movement authority at all.
        // If not, we need to fall back to halt.
        if (!this->hasMovementAuthority())
        {
            hauptsignal->setSignalBild(HauptSignalBild::Halt);
            return;
        }
        else
        {
            // Retrieve movement authority.
            const auto& authority = this->movementAuthority();

            // Translate movement authority to Signalbild.
            // XXX This needs to handle Zs etc
            HauptSignalBild newSignalBild{HauptSignalBild::Halt};

            switch (authority.type())
            {
            case MovementAuthorityType::FullSpeed:
                newSignalBild = HauptSignalBild::Fahrt;
                break;
            case MovementAuthorityType::ReducedSpeed:
                newSignalBild = HauptSignalBild::LangsamFahrt;
                break;
            default:
                // Not implemented
                // clang-format off
                qCCritical(catInterlocking)
                    << "Signal '"
                    << this->identifier()
                    << "' can't handle new movement authority: Type not implemented yet"
                    << Qt::endl;
                // clang-format on

                return;
            }

            // Apply the new aspect to the Hauptsignalschirm
            hauptsignal->setSignalBild(newSignalBild);
        }
    }

    void SignalMast::setHauptSignalSchirm(HauptSignalSchirm* hauptSignal)
    {
        if (this->m_hauptSignal.has_value())
        {
            // We do not allow interchanging the haupt signal schirm.
            throw Core::RuntimeError("SignalMast '{}' already has a Hauptsignalschirm", this->identifier());
        }

        hauptSignal->setParent(this);

        this->m_hauptSignal = hauptSignal;

        // Listen to its aspect changing, in order to implement distant
        // signal blanking.
        // clang-format off
        connect(this->m_hauptSignal.value(), &HauptSignalSchirm::signalBildChanged,
                this, &SignalMast::onHauptSignalBildChanged);
        // clang-format on
    }

    void SignalMast::onHauptSignalBildChanged(HauptSignalBild signalBild)
    {
        // Update blanking state of any attached distant signal
        this->updateBlankingState();
    }

    void SignalMast::setVorSignalSchirm(VorSignalSchirm* vorSignal)
    {
        if (this->m_vorSignal.has_value())
        {
            this->m_vorSignal.value()->setParent(nullptr);
        }

        this->m_vorSignal = vorSignal;

        this->m_vorSignal.value()->setParent(this);

        // Perform initial blanking state update. Any further updates
        // are triggered by the main signal aspect changing.
        this->updateBlankingState();
    }

    void SignalMast::updateBlankingState()
    {
        // If we do not have a distant signal, nothing to be done here.
        if (!this->isDistantSignal())
        {
            return;
        }

        // If we have a distant signal but no main signal, theres also
        // nothing to be done - such separate distant signals are always
        // active.
        if (!this->isMainSignal())
        {
            return;
        }

        // Determine the Hauptsignalbild, and if it is set to Halt,
        // disable the distant signal that shares this mast.
        const auto hauptSignalBild = this->hauptSignalSchirm()->signalBild();
        const auto vorSignalBlanked = (hauptSignalBild == HauptSignalBild::Halt);

        // Set blanking state in the Vorsignalschirm.
        this->vorSignalSchirm()->setIsBlanked(vorSignalBlanked);
    }
}