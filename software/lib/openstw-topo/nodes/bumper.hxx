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
            virtual QVector<Port> outgoingPortsFor(Port incomingPort) const override;
        };
    }
}
