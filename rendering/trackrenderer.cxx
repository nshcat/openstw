#include "trackrenderer.hxx"

namespace Rendering
{
    TrackRenderer::TrackRenderer(Openstw::Simulation::Tile* tile) : TileComponentRenderer(tile)
    {
    }
}

void Rendering::TrackRenderer::render(QPainter* painter)
{
    if (this->m_tile->hasTrack())
    {
    }
}
