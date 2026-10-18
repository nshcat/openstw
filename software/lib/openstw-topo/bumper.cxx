#include "bumper.hxx"

namespace Openstw
{
    namespace Topology
    {
        Bumper::Bumper(const QString& identifier, QObject* parent)
            : Node{NodeType::Bumper, {Port::Center}, identifier, parent}
        {
        }

        QVector<Port> Bumper::outgoingPortsFor(Port incomingPort) const
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
