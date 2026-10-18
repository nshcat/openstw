#pragma once

#include "frontbacknode.hxx"

namespace Openstw
{
    namespace Topology
    {
        class Signal : public FrontBackNode
        {
        public:
            explicit Signal(const QString& identifier, QObject* parent = nullptr);
        };
    }
}
