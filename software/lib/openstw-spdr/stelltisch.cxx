#include "stelltisch.hxx"

namespace Openstw::SpDr
{
    Stelltisch::Stelltisch(Interlocking::Interlocking* interlocking, QObject* parent)
        : QObject{parent}, m_interlocking{interlocking}
    {
    }

    Interlocking::Interlocking* Stelltisch::interlocking() const
    {
        return this->m_interlocking;
    }
}
