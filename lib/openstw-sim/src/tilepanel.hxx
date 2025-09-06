#pragma once

#include "tile.hxx"
#include <QObject>
#include <vector>

namespace Openstw::Simulation
{
    /**
     * @brief The TilePanel class manages a two-dimensional grid of tiles,
     * representing the Stelltisch in a Stellwerk.
     */
    class TilePanel : public QObject
    {
        Q_OBJECT

    public:
        TilePanel(QObject* parent, const std::size_t width, const std::size_t height);

        TilePanel(const TilePanel&) = delete;
        TilePanel& operator=(const TilePanel&) = delete;

        TilePanel(TilePanel&&) = delete;
        TilePanel& operator=(TilePanel&&) = delete;

    public:
        static TilePanel* CreateFrom(QObject* parent, const pugi::xml_node&);

    public:
        std::size_t width() const;
        std::size_t height() const;

        Tile* operator[](const std::size_t x, const std::size_t y);
        Tile* operator[](const GridPosition position);

    protected:
        std::size_t linearIndex(const std::size_t x, const std::size_t y) const;
        void setTile(const GridPosition position, Tile* tile);

    protected:
        std::size_t m_width{};
        std::size_t m_height{};
        std::vector<Tile*> m_tiles{};
    };
}
