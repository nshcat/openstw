#include "tilegraphicsobject.hxx"
#include <QPainter>

TileGraphicsObject::TileGraphicsObject(Openstw::Simulation::Tile* tile) : m_tile(tile)
{
    // this->setEnabled(true);
    // this->setFlags(ItemIsFocusable | ItemIsSelectable);
    this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    this->m_testObj = new Rendering::TestGraphicsObject(this);
    this->m_testObj->setPos(25.0f, 25.0f);
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
