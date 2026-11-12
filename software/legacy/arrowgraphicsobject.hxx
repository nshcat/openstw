#pragma once

#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    class ArrowGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        enum class LabelBoxLocation
        {
            Top = 0,
            Bottom = 1
        };

        Q_OBJECT

    public:
        constexpr static qreal arrowTipOverDrawFactor = 0.20f;
        constexpr static qreal labelBoxWidth = 140.0f;
        constexpr static qreal labelBoxHeight = 20.0f;
        constexpr static qreal labelBoxBorderPadding = 12.0f;

    public:
        ArrowGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawArrow(QPainter* painter, Openstw::Simulation::ArrowDirection direction, const QRectF& location) const;
        void drawLabel(QPainter* painter, LabelBoxLocation location, const QString& label) const;
    };
}
