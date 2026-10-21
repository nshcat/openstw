#include "path.hxx"
#include "exception.hxx"

namespace Openstw
{
    namespace Topology
    {
        Path::Path()
        {
        }

        QSpan<Element* const> Path::elements() const
        {
            return {this->m_sequence.cbegin(), this->m_sequence.cend()};
        }

        qsizetype Path::length() const
        {
            return this->m_sequence.length();
        }

        void Path::addElement(Element* element)
        {
            if (!element)
            {
                throw RuntimeError("Nullptr passed");
            }

            if (this->length() == 0)
            {
                // First element has to be a node.
                if (!element->isNode())
                {
                    throw RuntimeError("First element in path needs to be a node");
                }
            }
            else
            {
                // Make sure elements are of alternating type.
                if (this->m_sequence.last()->category() == element->category())
                {
                    throw RuntimeError("Elements within path need to be of alternating category");
                }
            }

            this->m_sequence.append(element);
        }

        Node* Path::start() const
        {
            if (this->length() == 0)
            {
                throw RuntimeError("Path is empty");
            }

            return dynamic_cast<Node*>(this->m_sequence.first());
        }

        Node* Path::end() const
        {
            if (this->length() == 0)
            {
                throw RuntimeError("Path is empty");
            }

            auto* lastElement = this->m_sequence.last();

            // Verify that its actually a node. It might not be if we are
            // not done with constructing the path (even though it should not be
            // used in that case, but one never knows..)
            if (!lastElement->isNode())
            {
                throw RuntimeError("Last path element is not a node, path is probably not completely constructed yet");
            }

            return dynamic_cast<Node*>(lastElement);
        }
    }
}
