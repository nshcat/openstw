#include "signal.hxx"
#include "logging.hxx"
#include <QVector>
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    Signal::Signal(ElementType type, QStringView identifier, Topology::Signal* topoNode,
                   QSpan<const LockingReason> supportedLockingReasons, bool supportsRouteStart, QObject* parent)
        : LockableElement{type, identifier, supportedLockingReasons, parent},
          m_supportsRouteStart{supportsRouteStart},
          m_topoNode{topoNode}
    {
    }

    bool Signal::supportsRouteStart() const
    {
        return this->m_supportsRouteStart;
    }

    bool Signal::hasMovementAuthority() const
    {
        return this->m_movementAuthority.has_value();
    }

    const MovementAuthority& Signal::movementAuthority() const
    {
        // Make sure the signal actually has an active movement
        // authority to give out!
        if (!this->hasMovementAuthority())
        {
            throw Core::RuntimeError("Signal '{}' does not currently give out movement authority!", this->identifier());
        }

        return this->m_movementAuthority.value();
    }

    void Signal::revokeMovementAuthority()
    {
        if (!this->hasMovementAuthority())
        {
            // clang-format off
            qCWarning(catInterlocking)
                << "Encountered movement authority revoke on signal '"
                << this->identifier()
                << "', which has no active movement authority"
                << Qt::endl;
            // clang-format on

            return;
        }

        this->m_movementAuthority = std::nullopt;

        // Notify our clients of the change
        emit this->movementAuthorityChanged(this->m_movementAuthority);
    }

    void Signal::setMovementAuthority(const MovementAuthority& authority)
    {
        this->m_movementAuthority = {authority};

        // Notify our clients of the change
        emit this->movementAuthorityChanged(this->m_movementAuthority);
    }

    Topology::Signal* Signal::topologyNode() const
    {
        return this->m_topoNode;
    }
}