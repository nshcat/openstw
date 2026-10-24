#include "path.hxx"
#include "exception.hxx"
#include <QString>
#include <QTextStream>
#include <algorithm>

namespace Openstw
{
    namespace Topology
    {
        Path::Path()
        {
        }

        QSpan<const PathSegment> Path::segments() const
        {
            return {this->m_sequence.cbegin(), this->m_sequence.cend()};
        }

        qsizetype Path::length() const
        {
            return this->m_sequence.length();
        }

        void Path::addNode(const Node* node, Port entryPort, Port exitPort)
        {
            if (!node)
            {
                throw RuntimeError("Nullptr passed");
            }

            if (this->length() > 0)
            {
                // Make sure elements are of alternating type.
                if (this->m_sequence.last().isNode())
                {
                    throw RuntimeError("Elements within path need to be of alternating category");
                }
            }

            this->m_sequence.append(PathSegment{node, entryPort, exitPort});
        }

        void Path::addEdge(const Edge* edge, Port fromPort, Port toPort)
        {
            if (!edge)
            {
                throw RuntimeError("Nullptr passed");
            }

            if (this->length() > 0)
            {
                // Make sure elements are of alternating type.
                if (this->m_sequence.last().isEdge())
                {
                    throw RuntimeError("Elements within path need to be of alternating category");
                }
            }
            else
            {
                // One cant add an edge to an empty path, since a path needs to
                // to both start and end with a segment representing a node.
                throw RuntimeError("Cant add edge segment to empty path");
            }

            this->m_sequence.append(PathSegment{edge, fromPort, toPort});
        }

        const Node* Path::start() const
        {
            if (this->length() == 0)
            {
                throw RuntimeError("Path is empty");
            }

            return this->m_sequence.first().node();
        }

        const Node* Path::end() const
        {
            if (this->length() == 0)
            {
                throw RuntimeError("Path is empty");
            }

            const auto& lastElement = this->m_sequence.last();

            // Verify that its actually a node. It might not be if we are
            // not done with constructing the path (even though it should not be
            // used in that case, but one never knows..)
            if (!lastElement.isNode())
            {
                throw RuntimeError("Last path element is not a node, path is probably not completely constructed yet");
            }

            return lastElement.node();
        }

        void Path::reverse()
        {
            std::reverse(this->m_sequence.begin(), this->m_sequence.end());
        }

        QString Path::dumpGraph() const
        {
            QString content{};
            QTextStream contentStream{&content};

            contentStream << "graph G {" << Qt::endl;

            // First, list all node segments.
            for (const auto& element : this->m_sequence)
            {
                if (element.isNode())
                {
                    contentStream << element.node()->identifier() << ";" << Qt::endl;
                }
            }

            // Then, relate nodes via edges.
            for (auto elementIter = this->m_sequence.begin(); elementIter != this->m_sequence.end(); ++elementIter)
            {
                const auto& element = *elementIter;
                if (element.isEdge())
                {
                    const auto* edge = element.edge();

                    // We know for sure that there must be a preceeding and following
                    // node segment!
                    const auto* previousNode = std::prev(elementIter)->node();
                    const auto* nextNode = std::next(elementIter)->node();

                    // clang-format off
                    contentStream
                        << previousNode->identifier()
                        << "--"
                        << nextNode->identifier()
                        << "  [headlabel="
                        << portToString(element.toPort())
                        << ", taillabel="
                        << portToString(element.fromPort())
                        << ", weight="
                        << edge->weight()
                        << "];"
                        << Qt::endl;
                    // clang-format on                   
                }
            }

            contentStream << "}" << Qt::endl;

            return content;
        }
    }
}
