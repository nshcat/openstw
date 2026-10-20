#include "signal.hxx"

namespace Openstw
{
    namespace Topology
    {
        Signal::Signal(const QString& identifier, QObject* parent)
            : Signal{NodeType::Signal, identifier, parent}
        {
        }

        Signal::Signal(NodeType type, const QString& identifier, QObject* parent)
            : FrontBackNode{type, identifier, parent}
        {
        }
    }
}
