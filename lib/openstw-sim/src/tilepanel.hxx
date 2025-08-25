#pragma once

#include "tile.hxx"

#include <vector>

namespace Openstw::Simulation
{
    /**
     * @brief The TilePanel class manages a two-dimensional grid of tiles,
     * representing the Stelltisch in a Stellwerk.
     */
    class TilePanel
    {
    public:
        TilePanel(std::size_t width, std::size_t height);

    public:
        std::size_t width() const;
        std::size_t height() const;

        Tile& operator[](std::size_t x, std::size_t y);

    protected:
        std::size_t linearIndex(std::size_t x, std::size_t y) const;

    protected:
        std::size_t m_width{};
        std::size_t m_height{};
        std::vector<Tile> m_tiles{};
    };
}
