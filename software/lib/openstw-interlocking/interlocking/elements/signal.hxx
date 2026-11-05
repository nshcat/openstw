#pragma once

#include "../movementauthority.hxx"
#include "lockableelement.hxx"
#include <QObject>
#include <optional>

namespace Openstw::Interlocking
{
    // Base class for signal elements within the interlocking network.
    class Signal : public LockableElement
    {
        Q_OBJECT

        // XXX Signal needs reference to associated track section,
        // and needs to revoke its movement authority if track section is occupied.

    public:
        explicit Signal(ElementType type, QStringView identifier, QSpan<const LockingReason> supportedLockingReasons,
                        bool supportsRouteStart, QObject* parent = nullptr);

    public:
        // Whether a route can originate from this signal.
        bool supportsRouteStart() const;

        // Whether the signal currently is handing out a movement
        // authority to the next train.
        bool hasMovementAuthority() const;

        // The current movement authority.
        const MovementAuthority& movementAuthority() const;

    public:
        // Whether this signal is a main signal, meaning it can give
        // out movement authorities for Zugfahrten.
        virtual bool isMainSignal() const = 0;

        // Whether this signal is a shunting signal, and can give out
        // movement authorities for Rangierfahrten.
        virtual bool isShuntingSignal() const = 0;

        // Whether this signal shows the state of another signal.
        // This does not mean it cant be a main signal as well!
        // Some signals are both a main and distant signal.
        virtual bool isDistantSignal() const = 0;

        // If this signal is a distant signal, retrieves the main signal
        // it references. The movement authority of the associated main signal
        // can be retrieved from the returned object.
        virtual std::optional<Signal*> referencedSignal() const = 0;

    protected:
        // Revoke the signals current movement authority.
        void revokeMovementAuthority();

        // Set given movement authority as the one currently given out
        // by this signal.
        void setMovementAuthority(const MovementAuthority& authority);

    signals:
        // Emitted when the current movement authority given out by this
        // signal changes. If the signal is not allowing proceeding, the
        // parameter will be nullopt.
        void movementAuthorityChanged(std::optional<MovementAuthority> movementAuthority);

    protected:
        // Whether a route can begin at this signal.
        const bool m_supportsRouteStart;

        // The current movement authority, if any.
        std::optional<MovementAuthority> m_movementAuthority{};
    };
}
