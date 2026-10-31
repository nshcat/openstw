#include "edge.hxx"

namespace Openstw
{
    namespace Topology
    {
        Edge::Edge(EdgeType type, const QString& identifier, QObject* parent)
            : Element{ElementCategory::Edge, identifier, parent}, m_type{type}
        {
        }

        EdgeType Edge::type() const
        {
            return this->m_type;
        }
    }
}
