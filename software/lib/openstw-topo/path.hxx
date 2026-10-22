#pragma once

#include "element.hxx"
#include "node.hxx"
#include "pathsegment.hxx"
#include <QObject>
#include <QVector>

namespace Openstw
{
    namespace Topology
    {
        // Class representing a sequence of topology graph elements that make
        // up a path from A to B.
        // A path always starts and ends on a node, and in between nodes and
        // edges have to appear in an alternating fashion.
        class Path
        {
        public:
            Path();

        public:
            // Retrieve the sequence of segments making up this path.
            QSpan<const PathSegment> segments() const;

            // Length of the path, in number of elements.
            qsizetype length() const;

            // Add a path segment containing a node the end of the path, extending it.
            // Note that a node always has to follow an edge, and vice versa.
            void addNode(Node* node);

            // Add a path segment containing an edge the end of the path, extending it,
            // connected to port "from" on the previous node, and to port "to" on the
            // following node.
            // Note that a node always has to follow an edge, and vice versa.
            void addEdge(Edge* edge, Port from, Port to);

            // Retrieve node that is the start of the path.
            Node* start() const;

            // Retrieve node that is the end of the path.
            Node* end() const;

        protected:
            // We are using a vector here, since that allows us to
            // return a QSpan to the user (continuous memory).
            // (Even if that means that appending is costly)
            QVector<PathSegment> m_sequence{};
        };
    }
}
