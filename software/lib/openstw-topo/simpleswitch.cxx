#include "simpleswitch.hxx"

namespace Openstw
{
    namespace Topology
    {
        SimpleSwitch::SimpleSwitch(const QString& identifier, QObject* parent)
            : Node{NodeType::SimpleSwitch, {Port::Wurzel, Port::Gerade, Port::Abzweig}, identifier, parent}
        {
        }

        QVector<Port> SimpleSwitch::outgoingPortsFor(Port incomingPort) const
        {
            // Possible outgoing nodes depends on the direction we are entering
            // the switch node.
            switch (incomingPort)
            {
            case Port::Wurzel:
                // Entering via the Wurzel of the switch allows branching -
                // either the Gerade or the Abzweig directions can be taken.
                return {Port::Gerade, Port::Abzweig};

            case Port::Abzweig:
            case Port::Gerade:
                // Entering from either Gerade or Abzweig ports only allows
                // exiting via the Wurzel port.
                return {Port::Wurzel};

            default:
                throw std::invalid_argument("Invalid incoming port given");
            }
        }
    }
}
