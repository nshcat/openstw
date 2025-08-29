#include "tilecomponentgraphicsobject.hxx"
#include "../tilegraphicsobject.hxx"
#include <QPainter>

namespace Rendering
{
    TileComponentGraphicsObject::TileComponentGraphicsObject(TileGraphicsObject* parent)
        : QGraphicsObject(parent), m_tile{parent->tile()}, m_tileGraphicsObj{parent}
    {
    }

    void TileComponentGraphicsObject::setup()
    {
        const auto mouseButtons = this->acceptsMouseInput();
        this->setAcceptedMouseButtons(mouseButtons.value_or(Qt::MouseButton::NoButton));
        this->positionSelf();
    }

    TileGraphicsObject* TileComponentGraphicsObject::tileGraphicsObject() const
    {
        return this->m_tileGraphicsObj;
    }

    std::optional<Qt::MouseButtons> TileComponentGraphicsObject::acceptsMouseInput() const
    {
        return std::nullopt;
    }

    void TileComponentGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        // Always clip to the bounding rectangle to make painting diagonal items
        // easier (by being able to use rotated rectangles)
        // painter->setClipRect(this->boundingRect());

        // Enable antialiasing
        painter->setRenderHint(QPainter::Antialiasing);
    }
}
