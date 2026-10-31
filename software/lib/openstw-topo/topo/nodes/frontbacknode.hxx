#pragma once

#include "node.hxx"
#include <QList>

namespace Openstw
{
    namespace Topology
    {
        // Helper class that represents a topology graph node
        // that has two ports, Front and Back, and allows traversal in the
        // 'expected' way - meaning both from front to back and back to front.
        class FrontBackNode : public Node
        {
            Q_OBJECT

        public:
            explicit FrontBackNode(NodeType type, const QString& identifier, QObject* parent = nullptr);

        public:
            virtual QVector<Port> outgoingPortsFor(Port incomingPort) const override;
        };
    }
}
