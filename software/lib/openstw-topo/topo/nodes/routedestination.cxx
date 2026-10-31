#include "routedestination.hxx"

namespace Openstw
{
    namespace Topology
    {
        RouteDestination::RouteDestination(const QString& identifier, QObject* parent)
            : Signal{NodeType::RouteDestination, identifier, parent}
        {
        }
    }
}