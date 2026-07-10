#include "tileelement.hxx"

namespace Openstw::Simulation
{
    TileElement::TileElement(Tile* parent) : m_parent{parent}
    {
    }

    void TileElement::update(const float deltaTime)
    {
        return;
    }

    Tile* TileElement::parent()
    {
        return this->m_parent;
    }
}
