#pragma once

#include "node.hxx"

namespace Openstw
{
    namespace Topology
    {
        class Boundary : public Node
        {
        public:
            explicit Boundary(const QString& identifier, QObject* parent = nullptr);

        public:
            virtual QVector<Port> outgoing_ports_for(Port incomingPort) const override;
        };
    }
}
