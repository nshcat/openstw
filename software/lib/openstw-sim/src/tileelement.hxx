#pragma once

#include "base.hxx"

namespace Openstw::Simulation
{
    class Tile;

    class TileElement : public ISimulationObject
    {
    public:
        TileElement(Tile* parent);
        virtual ~TileElement() = default;

    public:
        virtual void update(const float deltaTime);

    public:
        Tile* parent();

    protected:
        Tile* m_parent;
    };
}
