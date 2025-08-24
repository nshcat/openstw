#pragma once

#include "../tilegraphicsobject.hxx"
#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

namespace Rendering
{

    class TrackGraphicsItem : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT
    public:
        TrackGraphicsItem(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;
    };

} // namespace Rendering
