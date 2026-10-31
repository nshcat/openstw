#pragma once

#include "frontbacknode.hxx"

namespace Openstw
{
    namespace Topology
    {
        // Base class for all signal nodes
        // that can be the beginning and end of a route.
        class Signal : public FrontBackNode
        {
            Q_OBJECT

        public:
            explicit Signal(const QString& identifier, QObject* parent = nullptr);

        protected:
            // Constructor meant for subclasses that want to specify their own
            // specific node type.
            explicit Signal(NodeType type, const QString& identifier, QObject* parent = nullptr);
        };
    }
}
