#pragma once

#include "node.hxx"

namespace Openstw
{
    namespace Topology
    {
        class Boundary : public Node
        {
            Q_OBJECT

        public:
            explicit Boundary(const QString& identifier, QObject* parent = nullptr);

        public:
            virtual QVector<Port> outgoingPortsFor(Port incomingPort) const override;
        };
    }
}
