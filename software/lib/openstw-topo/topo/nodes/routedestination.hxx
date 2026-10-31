#pragma once

#include "signal.hxx"

namespace Openstw
{
    namespace Topology
    {
        class RouteDestination : public Signal
        {
            Q_OBJECT

        public:
            explicit RouteDestination(const QString& identifier, QObject* parent = nullptr);
        };
    }
}
