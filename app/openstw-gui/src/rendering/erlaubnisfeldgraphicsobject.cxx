#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "erlaubnisfeldgraphicsobject.hxx"
#include "renderinghelpers.hxx"

namespace Rendering
{
    ErlaubsnisFeldGraphicsObject::ErlaubsnisFeldGraphicsObject(TileGraphicsObject* parent)
        : TileComponentGraphicsObject(parent)
    {
    }

    QRectF ErlaubsnisFeldGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void ErlaubsnisFeldGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void ErlaubsnisFeldGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasDirectionArrows())
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);
    }
}
