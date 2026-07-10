#pragma once

#include "renderinghelpers.hxx"
#include "tilecomponentgraphicsobject.hxx"
#include "tilerenderingconstants.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    class PlatformGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

        constexpr static QColor platformColor{0x75, 0x75, 0x75};

    public:
        PlatformGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawPlatform(QPainter* painter, const VerticalDirection location) const;
    };
}
