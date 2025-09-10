#pragma once

#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    /**
     * @brief Graphics object that renders a Zugstrassentaste
     */
    class ZSTGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal buttonToBorderPadding = 14.0f;
        constexpr static qreal buttonOuterDiameter = 21.f;
        constexpr static qreal buttonInnerDiameter = 12.f;
        constexpr static QColor buttonBorderColor = QColor{0x3e, 0x3e, 0x3e};
        constexpr static QColor buttonOuterColor = QColor{0x96, 0x96, 0x96};
        constexpr static QColor buttonInnerColor = QColor{0xFF, 0x00, 0x00};

    public:
        ZSTGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    protected:
        virtual void positionSelf() override;
        virtual std::optional<Qt::MouseButtons> acceptsMouseInput() const override;

    protected:
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    };
}
