#include <stdexcept>

#include "tilepanel.hxx"

namespace Openstw::Simulation
{
    TilePanel::TilePanel(const std::size_t width, const std::size_t height) : m_width(width), m_height(height)
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

    TilePanel TilePanel::CreateFrom(const pugi::xml_node& root)
    {
        // Retrieve tile grid size
        const auto width = root.attribute("width").as_int();
        const auto height = root.attribute("height").as_int();

        if (width <= 0 || height <= 0)
            throw std::runtime_error("Missing or invalid width or height in tile panel XML");

        TilePanel instance{static_cast<std::size_t>(width), static_cast<std::size_t>(height)};

        for (const auto& childNode : root.children())
        {
            if (childNode.name() == std::string{"Tile"})
            {
                Tile tile = Tile::CreateFrom(childNode);
                instance[tile.position()] = std::move(tile);
            }
        }

        return instance;
    }

    std::size_t TilePanel::width() const
    {
        return this->m_width;
    }

    std::size_t TilePanel::height() const
    {
        return this->m_height;
    }

    Tile& TilePanel::operator[](const std::size_t x, const std::size_t y)
    {
        if (x >= this->m_width || y >= this->m_height)
            throw std::runtime_error("Tile access index out of range");

        return this->m_tiles[this->linearIndex(x, y)];
    }

    Tile& TilePanel::operator[](const GridPosition position)
    {
        return this->operator[](position.x, position.y);
    }

    std::size_t TilePanel::linearIndex(const std::size_t x, const std::size_t y) const
    {
        return (y * this->m_width) + x;
    }
}
