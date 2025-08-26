#pragma once

#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    class TrackGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT
    public:
        constexpr static qreal trackWidth = 16.f;

    public:
        TrackGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;
    };

} // namespace Rendering
