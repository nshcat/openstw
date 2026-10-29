#include "lampposition.hxx"
#include "exception.hxx"

namespace Openstw::SpDr
{
    LampPosition::LampPosition(unsigned int x, unsigned int y)
        : m_positionX{x}, m_positionY{y}
    {
        // Check for bounds.
        if (x >= 5 || y >= 3)
        {
            throw Core::ArgumentError("Lamp position ({}, {}) is out of 5x3 bounds", x, y);
        }
    }

    unsigned int LampPosition::x() const
    {
        return this->m_positionX;
    }

    unsigned int LampPosition::y() const
    {
        return this->m_positionY;
    }
}
