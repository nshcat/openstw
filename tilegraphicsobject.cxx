#include "tilegraphicsobject.hxx"
#include <QGraphicsScene>
#include <QPainter>

TileGraphicsObject::TileGraphicsObject(Openstw::Simulation::Tile* tile) : m_tile(tile)
{
}

Openstw::Simulation::Tile* TileGraphicsObject::tile() const
{
    return this->m_tile;
}

QRectF TileGraphicsObject::boundingRect() const
{
    return QRectF{0.0f, 0.0f, TileGraphicsObject::tileWidth, TileGraphicsObject::tileHeight};
}

void TileGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QColor backgroundColor = QColor{0x96, 0x96, 0x96};
    QColor backgroundBorderColor = QColor{0x3e, 0x3e, 0x3e};

    painter->setBrush(QBrush{backgroundColor});
    painter->setPen(QPen{backgroundBorderColor, 1.5f});
    painter->drawRect(this->boundingRect());
}

void TileGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    this->QGraphicsItem::mousePressEvent(event);
}

void TileGraphicsObject::setup()
{
    this->createComponentRenderers();
}

void TileGraphicsObject::createComponentRenderers()
{
    this->m_trackRenderer = new Rendering::TrackGraphicsObject{this};
    this->m_trackRenderer->setup();
    this->scene()->addItem(this->m_trackRenderer);
}
