#pragma once

#include <QObject>
#include <topology.hxx>

namespace Openstw::Interlocking
{
    // Class representing an interlocking system that handles
    // all safety critical railway operations on a given
    // railway topology network.
    class Interlocking : public QObject
    {
        Q_OBJECT

    public:
        Interlocking(Topology::Topology* topology, QObject* parent = nullptr);

    public:
        // Topology object associated with the interlocking system
        Topology::Topology* topology() const;

    protected:
        Topology::Topology* m_topology;
    };
}
