#include <stdexcept>

#include "tilepanel.hxx"

namespace Openstw::Simulation
{
    TilePanel::TilePanel(std::size_t width, std::size_t height) : m_width(width), m_height(height)
    {
        this->m_tiles.resize(width * height);
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

        return this->m_tiles[(y * this->m_width) + x];
    }
}
