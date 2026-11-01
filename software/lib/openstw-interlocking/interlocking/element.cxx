#include "element.hxx"
#include "elementtype.hxx"

namespace Openstw::Interlocking
{
    Element::Element(ElementType type, QStringView identifier, QObject* parent)
        : QObject{parent}, m_type{type}, m_identifier{identifier}
    {
    }

    ElementType Element::type() const
    {
        return this->m_type;
    }

    QStringView Element::identifier() const
    {
        return {this->m_identifier};
    }

    bool Element::isSignal() const
    {
        return Interlocking::isSignal(this->type());
    }
}
