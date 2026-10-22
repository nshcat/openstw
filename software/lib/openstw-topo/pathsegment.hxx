#pragma once

#include "edge.hxx"
#include "element.hxx"
#include "node.hxx"
#include <optional>

namespace Openstw
{
    namespace Topology
    {
        // A single entry within a topology network path.
        class PathSegment
        {
        public:
            // Construct path segment that represents a node.
            PathSegment(Node* node);

            // Construct path segment that represents an edge.
            PathSegment(Edge* edge, Port fromPort, Port toPort);

        public:
            // Retrieve enum value describing whether this segment represents a node or an edge.
            ElementCategory category() const;

            // Whether this segment represents a node.
            bool isNode() const;

            // Retrieve node within the segment, if the segment represents a node.
            Node* node() const;

            // Whether this segment represents an edge.
            bool isEdge() const;

            // Retrieve edge within the segment, if the segment represents an edge.
            Edge* edge() const;

            // Retrieve port the edge is connected to at the previous node, if the segment represents an edge.
            Port fromPort() const;

            // Retrieve port the edge is connected to at the next node, if the segment represents an edge.
            Port toPort() const;

        protected:
            // Discriminator determining whether a node or an edge is stored
            // within this path segment.
            ElementCategory m_category;

            // The underlying element that makes up this segment.
            Element* m_segmentElement;

            // If this segment is an edge, the ports the edge connects
            // to on the previous and next node.
            // Node that the direction of these ports is always in the
            // direction of the path, which means that the order of the
            // ports can very well be different from the "from" and "to"
            // port properties attached to the graph edge!
            std::optional<Port> m_fromPort{};
            std::optional<Port> m_toPort{};
        };
    }
}
