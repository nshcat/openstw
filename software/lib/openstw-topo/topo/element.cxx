#include "element.hxx"

namespace Openstw
{
    namespace Topology
    {
        Element::Element(ElementCategory category, const QString& identifier, QObject* parent)
            : QObject{parent}, m_category{category}, m_identifier{identifier}
        {
        }

        const QString& Element::identifier() const
        {
            return this->m_identifier;
        }

        void Element::setIdentifier(const QString& identifier)
        {
            this->m_identifier = identifier;
        }

        bool Element::isNode() const
        {
            return this->category() == ElementCategory::Node;
        }

        bool Element::isEdge() const
        {
            return this->category() == ElementCategory::Edge;
        }

        ElementCategory Element::category() const
        {
            return this->m_category;
        }
    }
}
