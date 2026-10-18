#include "boundary.hxx"

namespace Openstw
{
    namespace Topology
    {
        Boundary::Boundary(const QString& identifier, QObject* parent)
            : Node{NodeType::Boundary, {Port::Center}, identifier, parent}
        {
        }

        QVector<Port> Boundary::outgoingPortsFor(Port incomingPort) const
        {
            if (incomingPort != Port::Center)
            {
                throw std::invalid_argument("Invalid incoming port given");
            }

            // Node always terminates further path search.
            return {};
        }
    }
}
