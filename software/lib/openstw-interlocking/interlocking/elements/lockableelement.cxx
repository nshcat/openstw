#include "lockableelement.hxx"
#include "../logging.hxx"

namespace Openstw::Interlocking
{
    LockableElement::LockableElement(ElementType type, QStringView identifier,
                                     QSpan<const LockingReason> supportedLockingReasons, QObject* parent)
        : Element{type, identifier, parent},
          m_supportedReasons{supportedLockingReasons.begin(), supportedLockingReasons.end()}
    {
    }

    bool LockableElement::locked() const
    {
        return this->m_isLocked;
    }

    std::optional<LockingReason> LockableElement::lockReason() const
    {
        return this->m_lockReason;
    }

    bool LockableElement::supportsLockReason(LockingReason reason) const
    {
        return this->m_supportedReasons.contains(reason);
    }

    bool LockableElement::canLock(LockingReason reason) const
    {
        return !this->locked() && this->supportsLockReason(reason);
    }

    bool LockableElement::lock(LockingReason reason)
    {
        // Check if we are already locked..
        if (this->locked())
        {
            // clang-format off
            qCFatal(catInterlocking)
                << "Can't lock element '"
                << this->identifier()
                << "' for reason '"
                << reason
                << "': Already locked for reason '"
                << *this->m_lockReason
                << "'"
                << Qt::endl;
            // clang-format on

            return false;
        }

        // Verify that this element actually supports
        // the requested locking reason.
        if (!this->supportsLockReason(reason))
        {
            // clang-format off
            qCFatal(catInterlocking)
                << "Can't lock element '"
                << this->identifier()
                << "' for reason '"
                << reason
                << "': Reason not supported by element"
                << Qt::endl;
            // clang-format on

            return false;
        }

        this->m_isLocked = true;
        this->m_lockReason = {reason};

        emit this->lockedChanged(this->m_isLocked, this->m_lockReason);

        return true;
    }

    void LockableElement::unlock()
    {
        this->m_isLocked = false;
        this->m_lockReason = {};

        emit this->lockedChanged(false, this->m_lockReason);
    }
}
