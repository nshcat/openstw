#include "signal.hxx"

namespace Openstw
{
    namespace Topology
    {
        Signal::Signal(const QString& identifier, QObject* parent)
            : FrontBackNode{NodeType::Signal, identifier, parent}
        {
        }
    }
}
