#pragma once

#include "elements/signal.hxx"
#include "routestate.hxx"
#include <QObject>

namespace Openstw::Interlocking
{
    // Class representing a Fahrstrasse.
    class Route : public QObject
    {
        // XXX Someday, we should use Processes to model the fact that locking
        // routes IRL takes time - the route would expose a SimulationProcess
        // that, when executed, takes a route in state Locking and gradually
        // locks the elements within that a route, and sets its state to Locked
        // at the end.

        Q_OBJECT

    public:
        explicit Route(Signal* begin, Signal* end, QObject* parent = nullptr);

    public:
        // Current state of the Fahrstrasse.
        RouteState state() const;

        // Whether the Fahrstrasse is fully locked.
        bool locked() const;

        // Whether the Fahrstrasse was already released.
        bool released() const;

    signals:
        // Emitted whenever the state of the route changes.
        void stateChanged(Openstw::Interlocking::RouteState newState);

    protected:
        // Update the routes state.
        void setState(RouteState newState);

    protected:
        // Current state of the Fahrstrasse.
        RouteState m_state{RouteState::Defined};

        // Signal that marks the beginning of the Fahrstrasse.
        Signal* m_begin;

        // Signal that marks the end of the Fahrstrasse.
        Signal* m_end;
    };
}
