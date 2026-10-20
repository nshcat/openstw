#include "frontbacknode.hxx"
#include <stdexcept>

namespace Openstw
{
    namespace Topology
    {
        FrontBackNode::FrontBackNode(NodeType type, const QString& identifier, QObject* parent)
            : Node{type, {Port::Front, Port::Back}, identifier, parent}
        {
        }

        QVector<Port> FrontBackNode::outgoingPortsFor(Port incomingPort) const
        {
            // Node simply lets traffic traverse itself in the direction its
            // coming from.
            switch (incomingPort)
            {
            case Port::Front:
                return {Port::Back};

            case Port::Back:
                return {Port::Front};

            default:
                throw std::invalid_argument("Invalid incoming port given");
            }
        }
    }
}
