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
        constexpr static qreal trackThickness = 16.f;
        constexpr static qreal trackIndicatorHeight = 6.0f;
        constexpr static qreal trackIndicatorWidth = 64.0f;

    public:
        TrackGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        static QColor colorForTrackState(const Openstw::Simulation::TrackState);
    };

} // namespace Rendering
