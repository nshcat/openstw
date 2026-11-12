#pragma once

#include "tilefeaturegraphicsobject.hxx"
#include <QObject>
#include <spdr/aperture.hxx>
#include <spdr/aperturestate.hxx>
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

    public:
        TrackIndicatorFeatureObject(const Openstw::SpDr::TrackIndicatorFeature* feature, TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected slots:
        // Called whenever the state of the occupancy indicator aperture changes
        void onIndicatorStateChanged(Openstw::SpDr::ApertureState newState);

    protected:
        void drawBesetztMelder(QPainter* painter, const QRectF& trackRect,
                               const Openstw::SpDr::Aperture* aperture) const;

    protected:
        // The feature we are rendering.
        const Openstw::SpDr::TrackIndicatorFeature* m_feature;

        // Aperture representing the occupancy indicator
        Openstw::SpDr::Aperture* m_indicatorAperture;
    };
}
