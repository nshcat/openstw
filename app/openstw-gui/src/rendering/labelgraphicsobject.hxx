#pragma once

#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    class LabelGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal labelBoxVertPadding = 18.0f;
        constexpr static qreal labelBoxHorzPadding = 22.0f;

    public:
        LabelGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;
    };
}
