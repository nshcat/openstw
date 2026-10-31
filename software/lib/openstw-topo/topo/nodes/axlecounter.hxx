#pragma once

#include "frontbacknode.hxx"

namespace Openstw
{
    namespace Topology
    {
        class AxleCounter : public FrontBackNode
        {
            Q_OBJECT

        public:
            explicit AxleCounter(const QString& identifier, QObject* parent = nullptr);
        };
    }
}
