#include "element.hxx"

namespace Openstw
{
    namespace Topology
    {
        Element::Element(const QString& identifier, QObject* parent)
            : QObject{parent}, m_identifier{identifier}
        {
        }

        const QString& Element::identifier() const
        {
            return this->m_identifier;
        }

        void Element::set_identifier(const QString& identifier)
        {
            this->m_identifier = identifier;
        }

    }
}
