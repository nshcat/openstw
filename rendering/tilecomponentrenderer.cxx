#include "tilecomponentrenderer.hxx"

namespace Rendering
{

    TileComponentRenderer::TileComponentRenderer(Openstw::Simulation::Tile* tile) : m_tile(tile)
    {
    }

    Openstw::Simulation::Tile* TileComponentRenderer::tile() const
    {
        return this->m_tile;
    }
}
