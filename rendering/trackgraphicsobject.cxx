#include <QPainter>

#include "tilerenderingconstants.hxx"
#include "trackgraphicsobject.hxx"

namespace Rendering
{

    TrackGraphicsObject::TrackGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF Rendering::TrackGraphicsObject::boundingRect() const
    {
        // We span the whole tile.
        return QRectF{0.0f, 0.0f, TileRenderingConstants::tileWidth, TileRenderingConstants::tileHeight};
    }

    void Rendering::TrackGraphicsObject::positionSelf()
    {
        this->setPos(0.0f, 0.0f);
    }

    void Rendering::TrackGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                               QWidget* widget)
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
