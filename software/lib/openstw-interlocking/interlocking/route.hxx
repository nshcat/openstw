#pragma once

#include "elements/signal.hxx"
#include "routesegment.hxx"
#include "routestate.hxx"
#include <QObject>
#include <QSpan>
#include <QVector>

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
        // Construct route from given sequence of route segments.
        // The sequence must start and end with a signal segment,
        // and must contain no other signal segments in between.
        // A route always starts in state `Defined`.
        explicit Route(QSpan<const RouteSegment> segments, QObject* parent = nullptr);

    public:
        // Current state of the Fahrstrasse.
        RouteState state() const;

        // Whether the Fahrstrasse is fully locked.
        bool locked() const;

        // Whether the Fahrstrasse was already released.
        bool released() const;

        // The sequence of route segments making up this route.
        QSpan<const RouteSegment> segments() const;

        // Reference to the signal this route begins at.
        Signal* beginSignal() const;

        // Reference to the signal this route ends at.
        Signal* endSignal() const;

    public:
        // Update the routes state.
        void setState(RouteState newState);

    signals:
        // Emitted whenever the state of the route changes.
        void stateChanged(Openstw::Interlocking::RouteState newState);

    protected:
        // Current state of the Fahrstrasse.
        RouteState m_state{RouteState::Defined};

        // Sequence of route segments making up the route.
        QVector<RouteSegment> m_segments;

        // Signal that marks the beginning of the Fahrstrasse.
        Signal* m_beginSignal;

        // Signal that marks the end of the Fahrstrasse.
        Signal* m_endSignal;
    };
}
