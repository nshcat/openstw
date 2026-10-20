#pragma once

#include "signal.hxx"

namespace Openstw
{
    namespace Topology
    {
        class RouteDestination : public Signal
        {
        public:
            explicit RouteDestination(const QString& identifier, QObject* parent = nullptr);
        };
    }
}
