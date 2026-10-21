#pragma once

#include "element.hxx"
#include "node.hxx"
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
            // Retrieve the sequence of elements making up this path.
            QSpan<Element* const> elements() const;

            // Length of the path, in number of elements.
            qsizetype length() const;

            // Add an element to the end of the sequence, extending it.
            // Note that a node always has to follow an edge, and vice versa.
            void addElement(Element* element);

            // Retrieve node that is the start of the path.
            Node* start() const;

            // Retrieve node that is the end of the path.
            Node* end() const;

        protected:
            // We are using a vector here, since that allows us to
            // return a QSpan to the user (continuous memory).
            // (Even if that means that appending is costly)
            QVector<Element*> m_sequence{};
        };
    }
}
