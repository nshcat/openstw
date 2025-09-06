#include <stdexcept>

#include "tilepanel.hxx"

namespace Openstw::Simulation
{
    TilePanel::TilePanel(QObject* parent, const std::size_t width, const std::size_t height)
        : QObject{parent}, m_width(width), m_height(height)
    {
        this->m_tiles.resize(width * height);
        for (std::size_t ix = 0; ix < width; ++ix)
        {
            for (std::size_t iy = 0; iy < height; ++iy)
            {
                Tile* emptyTile = new Tile(this);
                emptyTile->setPosition(GridPosition{ix, iy});
                this->m_tiles[this->linearIndex(ix, iy)] = emptyTile;
            }
        }
    }

    TilePanel* TilePanel::CreateFrom(QObject* parent, const pugi::xml_node& root)
    {
        // Retrieve tile grid size
        const auto width = root.attribute("width").as_int();
        const auto height = root.attribute("height").as_int();

        if (width <= 0 || height <= 0)
            throw std::runtime_error("Missing or invalid width or height in tile panel XML");

        TilePanel* instance = new TilePanel{parent, static_cast<std::size_t>(width), static_cast<std::size_t>(height)};

        for (const auto& childNode : root.children())
        {
            if (childNode.name() == std::string{"Tile"})
            {
                Tile* tile = Tile::CreateFrom(instance, childNode);

                Tile* oldEmptyTile = instance->operator[](tile->position());
                oldEmptyTile->deleteLater();

                instance->setTile(tile->position(), tile);
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

    Tile* TilePanel::operator[](const std::size_t x, const std::size_t y)
    {
        if (x >= this->m_width || y >= this->m_height)
            throw std::runtime_error("Tile access index out of range");

        return this->m_tiles[this->linearIndex(x, y)];
    }

    Tile* TilePanel::operator[](const GridPosition position)
    {
        return this->operator[](position.x, position.y);
    }

    std::size_t TilePanel::linearIndex(const std::size_t x, const std::size_t y) const
    {
        return (y * this->m_width) + x;
    }

    void TilePanel::setTile(const GridPosition position, Tile* tile)
    {
        if (position.x >= this->m_width || position.y >= this->m_height)
            throw std::runtime_error("Tile access index out of range");

        this->m_tiles[this->linearIndex(position.x, position.y)] = tile;
    }
}
