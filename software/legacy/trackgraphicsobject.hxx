#pragma once

#include "tilefeaturegraphicsobject.hxx"
#include <QObject>
#include <spdr/features/trackindicatorfeature.hxx>

class TileGraphicsObject;

namespace Rendering
{
    // Graphics object that represents a track indicator feature on a SpDr tile.
    class TrackIndicatorFeatureObject : public TileFeatureGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal trackThickness = 16.f;
        constexpr static qreal trackIndicatorHeight = 6.0f;
        constexpr static qreal trackIndicatorWidth = 64.0f;
        constexpr static qreal labelBoxVertPadding = 18.0f;
        constexpr static qreal labelBoxHorzPadding = 22.0f;

    public:
        TrackIndicatorFeatureObject(Openstw::SpDr::TrackIndicatorFeature* feature, TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    public:
        static QColor colorForTrackState(const Openstw::Simulation::TrackState);

    protected:
        virtual void positionSelf() override;

    protected:
        void drawLabel(QPainter* painter, const QString& label, const bool withAlternativeMelder,
                       const Openstw::Simulation::TrackState trackState) const;
        void drawBesetztMelder(QPainter* painter, const QRectF& trackRect,
                               const Openstw::Simulation::TrackState trackState) const;
    };

} // namespace Rendering
