#include "tilecomponentgraphicsobject.hxx"

namespace Rendering
{
    TileComponentGraphicsObject::TileComponentGraphicsObject(TileGraphicsObject* parent)
        : QGraphicsObject(parent), m_tile{parent->tile()}
    {
    }

    void TileComponentGraphicsObject::setup()
    {
        const auto mouseButtons = this->acceptsMouseInput();
        this->setAcceptedMouseButtons(mouseButtons.value_or(Qt::MouseButton::NoButton));

        this->positionSelf();
    }

    std::optional<Qt::MouseButtons> TileComponentGraphicsObject::acceptsMouseInput() const
    {
        return std::nullopt;
    }
}
