#pragma once

#include "../element.hxx"
#include "../lockingreason.hxx"
#include <QObject>
#include <QSet>
#include <QSpan>
#include <optional>

namespace Openstw::Interlocking
{
    // An element within the interlocking network that supports
    // being locked as part of a Fahrstraße.
    class LockableElement : public Element
    {
        Q_OBJECT

    public:
        explicit LockableElement(ElementType type, QStringView identifier,
                                 QSpan<const LockingReason> supportedLockingReasons, QObject* parent = nullptr);

    public:
        // Whether this element is currently locked because
        // it is part of a Fahrstraße.
        bool locked() const;

        // If the element is locked, the reason as to why it is.
        std::optional<LockingReason> lockReason() const;

        // Check whether this element supports locking for the given reason.
        bool supportsLockReason(LockingReason reason) const;

        // Check whether this element can, right now, be locked for given reason.
        bool canLock(LockingReason reason) const;

    public:
        // Lock this element, with given reasoning as to why.
        // Returns whether locking succeeded.
        bool lock(LockingReason reason);

        // Unlock this element.
        void unlock();

    signals:
        // Emitted whenever the locked state of this element changes.
        void lockedChanged(bool isLocked, std::optional<Openstw::Interlocking::LockingReason> lockReason);

    protected:
        // Whether this element is currently locked as part of a
        // Fahrstraße.
        bool m_isLocked{false};

        // If locked, specific reason as to why it is.
        std::optional<LockingReason> m_lockReason{};

        // The locking reasons this element supports.
        QSet<LockingReason> m_supportedReasons;
    };
}
