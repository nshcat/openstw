#include "node.hxx"
#include "element.hxx"
#include "elementcategory.hxx"
#include <QException>
#include <stdexcept>

namespace Openstw
{
    namespace Topology
    {
        Node::Node(NodeType type, QSpan<const Port> ports, const QString& identifier, QObject* parent)
            : Element{ElementCategory::Node, identifier, parent}, m_type{type}, m_ports{ports.cbegin(), ports.cend()}
        {
            // A node without any ports is not allowed
            if (this->m_ports.size() == 0)
            {
                throw std::invalid_argument("A topology graph node requires at least one port");
            }
        }

        bool Node::hasPort(Port port) const
        {
            return this->m_ports.contains(port);
        }

        QVector<Port> Node::ports() const
        {
            return {this->m_ports.cbegin(), this->m_ports.cend()};
        }

        NodeType Node::type() const
        {
            return this->m_type;
        }

        bool Node::canTraverse(Port entry, Port exit) const
        {
            // First, check whether the node even has the given ports..
            if (!this->hasPort(entry) || !this->hasPort(exit))
            {
                return false;
            }

            // Retrieve outgoing ports for given entry port
            const auto outgoingPorts = this->outgoingPortsFor(entry);

            // Check whether the requested exit port is among those.
            return outgoingPorts.contains(exit);
        }
    }
}
