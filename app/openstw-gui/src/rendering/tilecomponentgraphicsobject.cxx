#include "tilecomponentgraphicsobject.hxx"
#include "../tilegraphicsobject.hxx"
#include <QCursor>
#include <QPainter>

namespace Rendering
{
    TileComponentGraphicsObject::TileComponentGraphicsObject(TileGraphicsObject* parent, bool providesContextMenu)
        : QGraphicsObject(parent), IContextMenuProvider(), m_tile{parent->tile()}, m_tileGraphicsObj{parent},
          m_providesContextMenu{providesContextMenu}
    {
    }

    void TileComponentGraphicsObject::setup()
    {
        const auto mouseButtons = this->acceptsMouseInput();
        this->setAcceptedMouseButtons(mouseButtons.value_or(Qt::MouseButton::NoButton));
        if (mouseButtons.has_value())
            this->setCursor(Qt::PointingHandCursor);

        this->positionSelf();
        this->afterSetup();
    }

    TileGraphicsObject* TileComponentGraphicsObject::tileGraphicsObject() const
    {
        return this->m_tileGraphicsObj;
    }

    bool TileComponentGraphicsObject::providesContextMenu() const
    {
        return this->m_providesContextMenu;
    }

    std::optional<Qt::MouseButtons> TileComponentGraphicsObject::acceptsMouseInput() const
    {
        return std::nullopt;
    }

    void TileComponentGraphicsObject::afterSetup()
    {
        return;
    }

    void TileComponentGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        // Always clip to the bounding rectangle to make painting diagonal items
        // easier (by being able to use rotated rectangles)
        painter->setClipRect(this->boundingRect());

        // Enable antialiasing
        painter->setRenderHint(QPainter::Antialiasing);
    }
}
