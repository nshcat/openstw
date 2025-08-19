#include "tilegraphicsitem.hxx"
#include <QPainter>

TileGraphicsItem::TileGraphicsItem(Openstw::Simulation::Tile* tile) : m_tile(tile), m_trackRenderer(tile)
{
}

Openstw::Simulation::Tile* TileGraphicsItem::tile() const
{
    return this->m_tile;
}

QRectF TileGraphicsItem::boundingRect() const
{
    return QRectF{0.0f, 0.0f, TileGraphicsItem::tileWidth, TileGraphicsItem::tileHeight};
}

void TileGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QColor backgroundColor = QColor{0x96, 0x96, 0x96};
    QColor backgroundBorderColor = QColor{0x3e, 0x3e, 0x3e};

    painter->setBrush(QBrush{backgroundColor});
    painter->setPen(QPen{backgroundBorderColor, 1.5f});
    painter->drawRect(this->boundingRect());
}
