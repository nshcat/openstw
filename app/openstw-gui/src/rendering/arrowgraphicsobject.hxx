#pragma once

#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    class ArrowGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal arrowTipOverDrawFactor = 0.20f;

    public:
        ArrowGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawArrow(QPainter* painter, Openstw::Simulation::ArrowDirection direction, const QRectF& location) const;
    };
}
