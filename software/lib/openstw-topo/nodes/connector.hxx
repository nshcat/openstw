#pragma once

#include "frontbacknode.hxx"

namespace Openstw
{
    namespace Topology
    {
        // Class describing a connector node within the topology graph.
        class Connector : public FrontBackNode
        {
            Q_OBJECT

        public:
            explicit Connector(const QString& identifier, QObject* parent = nullptr);
        };
    }
}
