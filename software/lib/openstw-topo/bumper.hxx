#pragma once

#include "node.hxx"

namespace Openstw
{
    namespace Topology
    {
        class Bumper : public Node
        {
        public:
            explicit Bumper(const QString& identifier, QObject* parent = nullptr);

        public:
            virtual QVector<Port> outgoing_ports_for(Port incomingPort) const override;
        };
    }
}
