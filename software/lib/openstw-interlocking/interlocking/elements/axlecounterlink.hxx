#pragma once

#include "axlecounterfacing.hxx"
#include "axlecounteroperation.hxx"
#include <QObject>
#include <topo/nodes/axlecounter.hxx>

namespace Openstw::Interlocking
{
    // Helper object that assigns a counter operation to a traversal
    // signal emitted from a axle counter node.
    class AxleCounterLink : public QObject
    {
        Q_OBJECT

    public:
        AxleCounterLink(Topology::AxleCounter* axleCounter, AxleCounterFacing facing, QObject* parent = nullptr);

    signals:
        // Emitted whenever the axle counter has detected an axle
        // and has translated the traversal to a specific axle counter
        // operation.
        void triggered(Openstw::Interlocking::AxleCounterOperation operation);

    protected slots:
        // Handler for axle counter traversal signal.
        void onTraversed(Openstw::Topology::Port entryPort, Openstw::Topology::Port exitPort);

    protected:
        // The axle counter within the topology graph
        // that we listen to for traversal events.
        Topology::AxleCounter* m_axleCounter;

        // In what direction the associated axle counter
        // is facing in relation to the track section we
        // are interacting with.
        AxleCounterFacing m_facing;
    };
}
