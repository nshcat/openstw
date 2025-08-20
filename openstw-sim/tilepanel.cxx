#include <stdexcept>

#include "tilepanel.hxx"

namespace Openstw::Simulation
{
    TilePanel::TilePanel(std::size_t width, std::size_t height) : m_width(width), m_height(height)
    {
        this->m_tiles.resize(width * height);
        for (std::size_t ix = 0; ix < width; ++ix)
        {
            for (std::size_t iy = 0; iy < height; ++iy)
            {
                this->m_tiles[this->linearIndex(ix, iy)].setPosition(GridPosition{ix, iy});
            }
        }
    }

    std::size_t TilePanel::width() const
    {
        return this->m_width;
    }

    std::size_t TilePanel::height() const
    {
        return this->m_height;
    }

    Tile& TilePanel::operator[](std::size_t x, std::size_t y)
    {
        if (x >= this->m_width || y >= this->m_height)
            throw std::runtime_error("Tile access index out of range");

        return this->m_tiles[this->linearIndex(x, y)];
    }

    std::size_t TilePanel::linearIndex(std::size_t x, std::size_t y) const
    {
        return (y * this->m_width) + x;
    }
}
