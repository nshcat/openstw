#include "trackgraphicsitem.hxx"
#include "tilerenderingconstants.hxx"

namespace Rendering
{

    TrackGraphicsItem::TrackGraphicsItem(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF Rendering::TrackGraphicsItem::boundingRect() const
    {
        // We span the whole tile.
        return QRectF{0.0f, 0.0f, TileRenderingConstants::tileWidth, TileRenderingConstants::tileHeight};
    }

    void Rendering::TrackGraphicsItem::positionSelf()
    {
        this->setPos(0.0f, 0.0f);
    }

    void Rendering::TrackGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasTrack())
            return;
    }
}
