#include "pathsegment.hxx"
#include "exception.hxx"

namespace Openstw
{
    namespace Topology
    {
        PathSegment::PathSegment(const Node* node, Port entryPort, Port exitPort)
            : m_category{ElementCategory::Node},
              m_segmentElement{static_cast<const Element*>(node)},
              m_fromPort{entryPort},
              m_toPort{exitPort}
        {
        }

        PathSegment::PathSegment(const Edge* edge, Port fromPort, Port toPort)
            : m_category{ElementCategory::Edge},
              m_segmentElement{static_cast<const Element*>(edge)},
              m_fromPort{fromPort},
              m_toPort{toPort}
        {
        }

        ElementCategory PathSegment::category() const
        {
            return this->m_category;
        }

        bool PathSegment::isNode() const
        {
            return this->category() == ElementCategory::Node;
        }

        const Node* PathSegment::node() const
        {
            if (!this->isNode())
            {
                throw RuntimeError("Path segment does not represent a node");
            }

            return dynamic_cast<const Node*>(this->m_segmentElement);
        }

        Port PathSegment::entryPort() const
        {
            if (!this->isNode())
            {
                throw RuntimeError("Path segment does not represent a node");
            }

            return this->m_fromPort;
        }

        Port PathSegment::exitPort() const
        {
            if (!this->isNode())
            {
                throw RuntimeError("Path segment does not represent a node");
            }

            return this->m_toPort;
        }

        bool PathSegment::isEdge() const
        {
            return this->category() == ElementCategory::Edge;
        }

        const Edge* PathSegment::edge() const
        {
            if (!this->isEdge())
            {
                throw RuntimeError("Path segment does not represent an edge");
            }

            return dynamic_cast<const Edge*>(this->m_segmentElement);
        }

        Port PathSegment::fromPort() const
        {
            if (!this->isEdge())
            {
                throw RuntimeError("Path segment does not contain a from port because its not an edge");
            }

            return this->m_fromPort;
        }

        Port PathSegment::toPort() const
        {
            if (!this->isEdge())
            {
                throw RuntimeError("Path segment does not contain a to port because its not an edge");
            }

            return this->m_toPort;
        }
    }
}
