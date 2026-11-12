#pragma once

#include <QObject>
#include <spdr/aperture.hxx>
#include <spdr/aperturestate.hxx>
#include <spdr/features/hpsignalfeature.hxx>

#include "renderinghelpers.hxx"
#include "tilefeaturegraphicsobject.hxx"

class TileGraphicsObject;

namespace Rendering
{
    // Graphics object that represents an main signal feature on a SpDr tile.
    class HpSignalFeatureObject : public TileFeatureGraphicsObject
    {
        Q_OBJECT

    public: // == Metrics and color constants
        constexpr static qreal signalToBorderPadding = 20.0f;
        constexpr static qreal inactiveLampDiameter = 11.0f;
        constexpr static qreal activeLampDiameter = 14.0f;
        constexpr static qreal signalLampPadding = 2.5f;
        constexpr static qreal activeKennLampDiameter = 11.0f;
        constexpr static qreal inactiveKennLampDiameter = 8.0f;
        constexpr static qreal hpSpaceForLamp = HpSignalFeatureObject::activeLampDiameter;
        constexpr static qreal hpPaddingBetweenLamps = 0.66f * HpSignalFeatureObject::hpSpaceForLamp;
        constexpr static qreal hpVrPadding = 7.5f;

        constexpr static qreal mastThickness = 4.0f;
        constexpr static qreal mastBaseWidth = 7.5f;
        constexpr static qreal mastBaseHeight = 16.0f;

    public:
        HpSignalFeatureObject(const Openstw::SpDr::HpSignalFeature* feature, TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        // Calculate the rectangle of either the top or bottom horizontal
        // half of the tile.
        QRectF calculateAreaRect(VerticalDirection location) const;

        // Calculate the area where the signal will be drawn in, based on the
        // feature layers orientation attribute.
        QRectF calculateSignalArea() const;

        // Draw the contents of the signal area. This includes the Signalschirm,
        // the mast, and the mast base, as well as other associated parts of the Signalmast.
        void drawSignalArea(QPainter* painter, const QRectF& location, const bool rotated) const;

        // Draw the mast base.
        void drawMastBase(QPainter* painter, const QRectF& location) const;

        // Draw mast segment.
        void drawMastSegment(QPainter* painter, const QRectF& location) const;

        // Draw a single signal lamp.
        void drawSignalLamp(QPainter* painter, const QRectF& location, const Openstw::SpDr::Aperture* aperture,
                            const qreal onDiameter, const qreal offDiameter) const;

        // Determine the dimensions of the Hauptsignalschirm.
        QSizeF measureHauptSignal() const;

        // Actually draw the Hauptsignalschirm.
        void drawHauptSignal(QPainter* painter, const QRectF& location) const;

    protected slots:
        // Called whenever the state of any of the features apertures changes.
        // Will trigger a redraw of the whole feature.
        void onApertureStateChanged(Openstw::SpDr::ApertureState newState);

    protected:
        // The feature we are rendering.
        const Openstw::SpDr::HpSignalFeature* m_feature;

        // Aperture representing the red signal lamp.
        Openstw::SpDr::Aperture* m_redLamp;

        // Aperture representing the green signal lamp.
        Openstw::SpDr::Aperture* m_greenLamp;
    };
}
