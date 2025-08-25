#include <QPainter>

#include "../tilegraphicsobject.hxx"
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

        painter->save();

        painter->translate(TileRenderingConstants::halfTileWidth, TileRenderingConstants::halfTileHeight);
        painter->rotate(TileRenderingConstants::tileDiagonalAngle);
        painter->translate(-TileRenderingConstants::halfTileWidth, -TileRenderingConstants::halfTileHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);
        painter->drawRect(QRectF{-40.0f, TileRenderingConstants::halfTileHeight - 8.0f,
                                 TileRenderingConstants::tileWidth + 80, 16.0f});

        painter->restore();

        painter->save();

        painter->translate(TileRenderingConstants::halfTileWidth, TileRenderingConstants::halfTileHeight);
        painter->rotate(-TileRenderingConstants::tileDiagonalAngle);
        painter->translate(-TileRenderingConstants::halfTileWidth, -TileRenderingConstants::halfTileHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);
        painter->drawRect(QRectF{-40.0f, TileRenderingConstants::halfTileHeight - 8.0f,
                                 TileRenderingConstants::tileWidth + 80, 16.0f});

        painter->restore();
    }
}
