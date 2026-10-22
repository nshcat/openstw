#include "path.hxx"
#include "exception.hxx"

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

        void Path::addNode(Node* node)
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

            this->m_sequence.append(PathSegment{node});
        }

        Node* Path::start() const
        {
            if (this->length() == 0)
            {
                throw RuntimeError("Path is empty");
            }

            return this->m_sequence.first().node();
        }

        Node* Path::end() const
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
    }
}
