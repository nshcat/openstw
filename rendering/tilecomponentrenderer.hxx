#pragma once

#include <QPainter>
#include <tile.hxx>

#include "tilerenderingconstants.hxx"

namespace Rendering
{
    /**
     * @brief Abstract base class for all classes that render components of a tile.
     */
    class TileComponentRenderer
    {
    public:
        TileComponentRenderer(Openstw::Simulation::Tile* tile);

    public:
        Openstw::Simulation::Tile* tile() const;

    public:
        virtual void render(QPainter* painter) = 0;

    protected:
        Openstw::Simulation::Tile* m_tile{};
    };

} // namespace Rendering
