#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "platformgraphicsobject.hxx"
#include "renderinghelpers.hxx"

namespace Rendering
{

    PlatformGraphicsObject::PlatformGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF PlatformGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void PlatformGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void PlatformGraphicsObject::drawPlatform(QPainter* painter, const VerticalDirection location) const
    {
        painter->save();

        const auto platformRect = this->tileGraphicsObject()->horizontalDrawingAreaRect(location);

        painter->setPen(rectanglePen(PlatformGraphicsObject::platformColor, 1.5f));
        painter->setBrush(PlatformGraphicsObject::platformColor);
        painter->drawRect(adjustRectForBorder(platformRect, 1.0f));

        painter->restore();
    }

    void PlatformGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasPlatform())
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto& platforms = this->m_tile->platforms();

        if (platforms.has(Openstw::Simulation::Platform::Top))
            this->drawPlatform(painter, VerticalDirection::Top);

        if (platforms.has(Openstw::Simulation::Platform::Bottom))
            this->drawPlatform(painter, VerticalDirection::Bottom);
    }
}
