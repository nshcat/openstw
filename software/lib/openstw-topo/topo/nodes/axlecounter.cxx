#include "axlecounter.hxx"

namespace Openstw
{
    namespace Topology
    {
        AxleCounter::AxleCounter(const QString& identifier, QObject* parent)
            : FrontBackNode{NodeType::AxleCounter, identifier, parent}
        {
        }
    }
}
