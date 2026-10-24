#pragma once

#include "node.hxx"

namespace Openstw
{
    namespace Topology
    {
        class SimpleSwitch : public Node
        {
            Q_OBJECT

        public:
            explicit SimpleSwitch(const QString& identifier, QObject* parent = nullptr);

        public:
            virtual QVector<Port> outgoingPortsFor(Port incomingPort) const override;
        };
    }
}
