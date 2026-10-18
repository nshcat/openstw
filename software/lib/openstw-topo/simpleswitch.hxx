#pragma once

#include "node.hxx"

namespace Openstw
{
    namespace Topology
    {
        class SimpleSwitch : public Node
        {
        public:
            explicit SimpleSwitch(const QString& identifier, QObject* parent = nullptr);

        public:
            virtual QVector<Port> outgoing_ports_for(Port incomingPort) const override;
        };
    }
}
