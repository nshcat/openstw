#pragma once

#include "tilecomponentrenderer.hxx"

namespace Rendering
{

    class TrackRenderer : public Rendering::TileComponentRenderer
    {
    public:
        TrackRenderer(Openstw::Simulation::Tile* tile);

    public: // == TileComponentRenderer implementation
        virtual void render(QPainter* painter) override;
    };

} // namespace Rendering
