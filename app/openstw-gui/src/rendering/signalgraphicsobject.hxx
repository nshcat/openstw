#pragma once

#include <QObject>

#include <enums.hxx>
#include <hauptsignalschirm.hxx>
#include <isignalschirm.hxx>
#include <zugdeckungssignalschirm.hxx>

#include "fsmgraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "tilecomponentgraphicsobject.hxx"

namespace Rendering
{
    class SignalGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        enum class HauptSignalRenderingStyle
        {
            Normal = 0,
            Compact = 1 //< Used if a signal has two Signalschirme. The kennlicht is inside the red lamp.
        };

        Q_OBJECT

    public: // == Metrics and color constants
        constexpr static qreal signalToBorderPadding = 20.0f;
        constexpr static QColor kennLichtColor{0xFE, 0xFF, 0x7F};
        constexpr static QColor inactiveLampColor{0x23, 0x23, 0x23};
        constexpr static qreal inactiveLampDiameter = 11.0f;
        constexpr static qreal activeLampDiameter = 14.0f;
        constexpr static qreal signalLampPadding = 2.5f;
        constexpr static qreal activeKennLampDiameter = 11.0f;
        constexpr static qreal inactiveKennLampDiameter = 8.0f;
        constexpr static qreal hpSpaceForLamp = SignalGraphicsObject::activeLampDiameter;
        constexpr static qreal hpPaddingBetweenLamps = 0.66f * SignalGraphicsObject::hpSpaceForLamp;
        constexpr static qreal hpVrPadding = 7.5f;

        constexpr static qreal vrActiveLampDiameter = 8.5f;
        constexpr static qreal vrInactiveLampDiameter = 4.0f;
        constexpr static qreal vrSpaceForLamp = SignalGraphicsObject::vrActiveLampDiameter;
        constexpr static qreal vrLampPaddingX = 0.75f;
        constexpr static qreal vrLampPaddingY = 0.0f;
        constexpr static qreal vrExtraSpaceForKennLicht = 22.0f;

        constexpr static qreal mastThickness = 4.0f;
        constexpr static qreal mastBaseWidth = 7.5f;
        constexpr static qreal mastBaseHeight = 16.0f;

        constexpr static qreal sperrMelderDiameter = 11.0f;
        constexpr static QColor sperrMelderInactiveColor = inactiveLampColor;
        constexpr static QColor sperrMelderActiveColor{0xFF, 0xFF, 0x0};
        constexpr static qreal sperrMelderToBorderPadding = 20.0f;

        constexpr static qreal sperrMelderMastBasePadding = 1.0f;

        constexpr static qreal labelWidth = 45.0f;
        constexpr static qreal labelHeight = 18.0f;

        constexpr static qreal labelPaddingCenteredFSM = signalToBorderPadding;
        constexpr static qreal labelPaddingAlignedFSM =
            FSMGraphicsObject::feststellMelderPaddingToBorder + FSMGraphicsObject::feststellMelderSideLength + 10.0f;

        constexpr static qreal dwegMelderDiameter = 14.0f;
        constexpr static QColor dwegMelderInactiveColor = inactiveLampColor;
        constexpr static QColor dwegMelderActiveColor{0xFF, 0xFF, 0x0};
        constexpr static qreal dwegMelderPaddingToBorder = 16.0f;

    public:
        SignalGraphicsObject(TileGraphicsObject* parent, const Openstw::Simulation::TileElementDirection direction);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        QRectF calculateAreaRect(VerticalDirection location) const;
        QRectF calculateSignalArea() const;
        QRectF calculateMelderArea() const;

        void drawSignalArea(QPainter* painter, const QRectF& location, const bool rotated) const;
        void drawMelderArea(QPainter* painter, const QRectF& location, const bool rotated) const;

        void drawSperrMelder(QPainter* painter, const QRectF& location,
                             const Openstw::Simulation::SperrMelderState state) const;

        void drawDWegMelder(QPainter* painter, const QRectF& location,
                            const Openstw::Simulation::StaticLampState state) const;

        void drawLabel(QPainter* painter, const QRectF& location, const QString& label) const;

        void drawMastBase(QPainter* painter, const QRectF& location) const;
        void drawMastSegment(QPainter* painter, const QRectF& location) const;

        void drawSignalLamp(QPainter* painter, const QRectF& location, const QColor color, const qreal diameter) const;

        QSizeF measureHauptSignal(HauptSignalRenderingStyle style,
                                  const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm) const;
        void drawHauptSignal(QPainter* painter, HauptSignalRenderingStyle style,
                             const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm,
                             const QRectF& location) const;

        QSizeF measureVorSignal(const Openstw::Simulation::VorSignalSchirm* vorSignalSchirm) const;
        void drawVorSignal(QPainter* painter, const Openstw::Simulation::VorSignalSchirm* vorSignalSchirm,
                           const QRectF& location) const;

        QSizeF measureZugDeckungsSignal(
            const Openstw::Simulation::ZugDeckungsSignalSchirm* zugDeckungsSignalSchirm) const;
        void drawZugDeckungsSignal(QPainter* painter,
                                   const Openstw::Simulation::ZugDeckungsSignalSchirm* zugDeckungsSignalSchirm,
                                   const QRectF& location) const;

    protected:
        Openstw::Simulation::TileElementDirection m_direction;
    };
}
