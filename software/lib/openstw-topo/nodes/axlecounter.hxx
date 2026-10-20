#pragma once

#include "frontbacknode.hxx"

namespace Openstw
{
    namespace Topology
    {
        class AxleCounter : public FrontBackNode
        {
        public:
            explicit AxleCounter(const QString& identifier, QObject* parent = nullptr);
        };
    }
}
