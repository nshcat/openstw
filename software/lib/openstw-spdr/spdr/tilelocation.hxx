#pragma once

#include <functional>

namespace Openstw::SpDr
{
    // Location of a single tile on the Stelltisch.
    class TileLocation
    {
    public:
        TileLocation(unsigned x, unsigned y);

    public:
        // X position of the tile on the Stelltisch
        unsigned x() const;

        // Y position of the tile on the Stelltisch
        unsigned y() const;

    public:
        bool operator==(const TileLocation& rhs) const = default;
        bool operator<=>(const TileLocation& rhs) const = default;

    protected:
        unsigned m_posX;
        unsigned m_posY;
    };
}

// Hash implementation for tile locations
template <>
struct std::hash<Openstw::SpDr::TileLocation>
{
    std::size_t operator()(Openstw::SpDr::TileLocation const& location) const noexcept;
};