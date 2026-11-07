#pragma once

#include "../movementauthority.hxx"
#include "lockableelement.hxx"
#include <QObject>
#include <optional>
#include <topo/nodes/signal.hxx>

namespace Openstw::Interlocking
{
    // Base class for signal elements within the interlocking network.
    class Signal : public LockableElement
    {
        Q_OBJECT

    public:
        explicit Signal(ElementType type, QStringView identifier, Topology::Signal* topoNode,
                        QSpan<const LockingReason> supportedLockingReasons, bool supportsRouteStart,
                        QObject* parent = nullptr);

    public:
        // Whether a route can originate from this signal.
        bool supportsRouteStart() const;

        // Whether the signal currently is handing out a movement
        // authority to the next train.
        bool hasMovementAuthority() const;

        // The current movement authority.
        const MovementAuthority& movementAuthority() const;

        // The signal node within the topology graph this signal corresponds to.
        Topology::Signal* topologyNode() const;

    public: // The processes dealing with routes interact with signals
            // via these methods - they dont care about specific Signalbilder,
            // its the job of the specific signal sub class to figure out
            // how to display an abstract movement authority.
        // Revoke the signals current movement authority.
        void revokeMovementAuthority();

        // Set given movement authority as the one currently given out
        // by this signal.
        void setMovementAuthority(const MovementAuthority& authority);

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
        virtual std::optional<Signal*> associatedSignal() const = 0;

        // If this signal is a distant signal, whether it currently has a main
        // signal associated with it.
        virtual bool hasAssociatedSignal() const = 0;

    public:
        // If this signal is a distant signal, associate it with given main
        // signal. The distant signal will start deriving its aspect from that
        // main signal.
        virtual void associateWithSignal(Signal* mainSignal) = 0;

        // Revoke an association to a main signal established with `associateWithSignal`.
        // The signal will start showing a default halt aspect (or be set to off, depending
        // on the specific signal)
        virtual void revokeSignalAssociation() = 0;

    protected:
        // Signal to both the sub class as well as to any connected clients
        // that the current movement authority given out by this signal has changed.
        void signalMovementAuthorityChanged();

    protected:
        // This method is called by the signal base class itself, and gives
        // any derived, specific signal class a chance to derive its aspect
        // from the new movement authority (or lack thereof)
        virtual void onMovementAuthorityChanged() = 0;

    signals:
        // Emitted when the current movement authority given out by this
        // signal changes. If the signal is not allowing proceeding, the
        // parameter will be nullopt.
        void movementAuthorityChanged(std::optional<MovementAuthority> movementAuthority);

    protected:
        // The signal node inside the topology graph this signal
        // corresponds to.
        Topology::Signal* m_topoNode;

        // Whether a route can begin at this signal.
        const bool m_supportsRouteStart;

        // The current movement authority, if any.
        std::optional<MovementAuthority> m_movementAuthority{};
    };
}
