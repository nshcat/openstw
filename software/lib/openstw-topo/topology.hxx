#pragma once

#include <QMap>
#include <QObject>
#include <boost/graph/adjacency_list.hpp>

namespace Openstw::Topology
{
    // Class containing the topology graph, which describes
    // the physical layout of the railway network.
    // It holds direct ownership of all node and edge object instances,
    // the graph is just a an adjacent view on those objects that describes
    // their relationships.
    class Topology : public QObject
    {
        Q_OBJECT;

    public:
        Topology(QObject* parent);

    protected:
    };
}
