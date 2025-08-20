#include <QPainter>

#include "testgraphicsobject.hxx"

namespace Rendering
{

    TestGraphicsObject::TestGraphicsObject(QGraphicsItem* parent) : QGraphicsObject(parent)
    {
    }

    TestGraphicsObject::~TestGraphicsObject()
    {
    }

    QRectF TestGraphicsObject::boundingRect() const
    {
        return QRectF{0.0f, 0.0f, 30.0f, 30.0f};
    }

    void TestGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        painter->setBackground(QBrush{Qt::green});
        painter->setPen(QPen{Qt::red, 1.0f});
        painter->drawRect(this->boundingRect());
    }

    void TestGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        this->QGraphicsItem::mousePressEvent(event);
    }

}
