#pragma once

#include <cstdint>

namespace Openstw::SpDr
{
    // Class describing the position of a single lamp
    // within the 5x3 grid of a SpDr Stelltisch tile.
    // This can be used to implement realistic, physical replicas
    // of a Stelltisch.
    // Note that a single display element might correspond to multiple
    // actual lamps!
    class LampPosition
    {
    public:
        // Describe a lamp installed at position (x, y) within
        // the 5x3 grid of a SpDr tile.
        LampPosition(unsigned x, unsigned y);

    public:
        unsigned x() const;
        unsigned y() const;

    protected:
        unsigned m_positionX{};
        unsigned m_positionY{};
    };
}