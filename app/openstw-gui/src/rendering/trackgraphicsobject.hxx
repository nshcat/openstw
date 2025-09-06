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
        constexpr static qreal labelBoxVertPadding = 18.0f;
        constexpr static qreal labelBoxHorzPadding = 22.0f;

    public:
        TrackGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawLabel(QPainter* painter, const QString& label, const bool withAlternativeMelder,
                       const Openstw::Simulation::TrackState trackState) const;
        void drawBesetztMelder(QPainter* painter, const QRectF& trackRect,
                               const Openstw::Simulation::TrackState trackState) const;

    protected:
        static QColor colorForTrackState(const Openstw::Simulation::TrackState);
    };

} // namespace Rendering
