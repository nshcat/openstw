#include "connector.hxx"

namespace Openstw
{
    namespace Topology
    {
        Connector::Connector(const QString& identifier, QObject* parent)
            : FrontBackNode{NodeType::Connector, identifier, parent}
        {
        }
    }
}
