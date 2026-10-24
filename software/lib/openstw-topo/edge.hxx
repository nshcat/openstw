#pragma once

#include "edgetype.hxx"
#include "element.hxx"

namespace Openstw
{
    namespace Topology
    {
        // Base class for edges within the topology graph.
        class Edge : public Element
        {
            Q_OBJECT

        public:
            Edge(EdgeType type, const QString& identifier, QObject* parent = nullptr);

        public:
            EdgeType type() const;
            virtual double weight() const = 0;

        protected:
            EdgeType m_type;
        };
    }
}
