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

    std::optional<Signal*> SignalMast::referencedSignal() const
    {
        // If we do not have a Vorsignalschirm, we dont have a referenced
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

    void SignalMast::setHauptSignalSchirm(HauptSignalSchirm* hauptSignal)
    {
        if (this->m_hauptSignal.has_value())
        {
            // We do not allow interchanging the haupt signal schirm.
            throw Core::RuntimeError("SignalMast '{}' already has a Hauptsignalschirm", this->identifier());
        }

        hauptSignal->setParent(this);

        this->m_hauptSignal = hauptSignal;

        // Listen to signal notifying us that the Hauptsignalschirms displayed
        // aspect has changed, in order to derive the current movement authority from it.
        // clang-format off
        connect(this->m_hauptSignal.value(), &HauptSignalSchirm::signalBildChanged,
                this, &SignalMast::handleHauptSignalBildChanged);
        // clang-format on
    }

    void SignalMast::setVorSignalSchirm(VorSignalSchirm* vorSignal)
    {
        if (this->m_vorSignal.has_value())
        {
            this->m_vorSignal.value()->setParent(nullptr);
        }

        this->m_vorSignal = vorSignal;

        this->m_vorSignal.value()->setParent(this);
    }

    void SignalMast::handleHauptSignalBildChanged(HauptSignalBild newSignalBild)
    {
        // First, check if the Signalbild allows passage. If it doesnt,
        // revoke any existing movement authority.
        if (newSignalBild == HauptSignalBild::Halt)
        {
            // Check if we need to revoke a movement authority.
            if (this->hasMovementAuthority())
            {
                // If so, do so.
                this->revokeMovementAuthority();
            }

            return;
        }

        // Translate the Signalbild into a movement authority.
        // XXX Deal with possible Zs and such on this mast.
        std::optional<MovementAuthority> authority{};

        if (newSignalBild == HauptSignalBild::Fahrt)
        {
            authority = MovementAuthority::FullSpeed();
        }
        else if (newSignalBild == HauptSignalBild::LangsamFahrt)
        {
            // XXX This depends on Zs and such..
            authority = MovementAuthority::ReducedSpeed(40.0);
        }

        // Validate that we sucessfully derived a movement authority.
        if (!authority.has_value())
        {
            // Something went wrong..
            // clang-format off
            qCCritical(catInterlocking())
                << "SignalMast '"
                << this->identifier()
                << "' failed to derive movement authority from HauptSignalBild '"
                << newSignalBild
                << "'"
                << Qt::endl;
            // clang-format on

            return;
        }

        // If so, apply it.
        this->setMovementAuthority(authority.value());
    }
}