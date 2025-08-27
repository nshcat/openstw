#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "tilerenderingconstants.hxx"
#include "trackgraphicsobject.hxx"

namespace Rendering
{

    TrackGraphicsObject::TrackGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF TrackGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void TrackGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void TrackGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        if (!this->m_tile->hasTrack())
            return;

        const auto boundingRect = this->boundingRect();

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(
            QRectF{boundingRect.left(), boundingRect.top(), boundingRect.width(), TrackGraphicsObject::trackThickness},
            1.0f));
    }
}
