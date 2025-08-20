#pragma once

#include <QGraphicsObject>
#include <QObject>
#include <QWidget>

namespace Rendering
{

    class TestGraphicsObject : public QGraphicsObject
    {
        Q_OBJECT

    public:
        TestGraphicsObject(QGraphicsItem* parent);
        ~TestGraphicsObject();

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    };

} // namespace Rendering
