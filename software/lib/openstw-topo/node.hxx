#pragma once

#include "element.hxx"
#include "nodetype.hxx"
#include "port.hxx"
#include <QSet>
#include <QSpan>
#include <QVector>

namespace Openstw
{
    namespace Topology
    {
        // Base class for all topology graph nodes.
        // Each type of node knows what kinds of ports it has,
        // which is used during validation of topology input data
        // before constructing the graph. This makes sure that no
        // invalid edges can exist within the graph.
        class Node : public Element
        {
            Q_OBJECT

        public:
            explicit Node(NodeType type, QSpan<const Port> ports, const QString& identifier, QObject* parent = nullptr);

        public:
            bool hasPort(Port port) const;
            NodeType type() const;
            QVector<Port> ports() const;

            // Check whether the node can be left from given exit port, after it was
            // entered via given entry port.
            bool canTraverse(Port entry, Port exit) const;

            // Traverse this node, notifying listeners about it.
            void traverse(Port entry, Port exit) const;

        public:
            // Determine which outgoing ports can be reached from given
            // incoming port.
            // For example, with a simple three-way switch, coming from
            // the Wurzel port, both the elements connected to the
            // Gerade and Abzweig ports are reachable; whereas coming
            // from the Gerade or Abzweig ports, only the Wurzel port
            // is reachable.
            virtual QVector<Port> outgoingPortsFor(Port incomingPort) const = 0;

        signals:
            // Emitted whenever an entity traversed the node.
            void traversed(Openstw::Topology::Port entryPort, Openstw::Topology::Port exitPort) const;

        protected:
            NodeType m_type;
            QSet<Port> m_ports;
        };
    }
}
