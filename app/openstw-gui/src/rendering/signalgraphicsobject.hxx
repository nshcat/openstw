#pragma once

#include "hauptsignalschirm.hxx"
#include "isignalschirm.hxx"
#include "tilecomponentgraphicsobject.hxx"
#include <QObject>
#include <enums.hxx>

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

    public:
        SignalGraphicsObject(TileGraphicsObject* parent, const Openstw::Simulation::TileElementDirection direction);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawSperrMelder(QPainter* painter, const QRectF& location,
                             const Openstw::Simulation::SperrMelderState state);

        void drawMastBase(QPainter* painter, const QRectF& location);
        void drawMastSegment(QPainter* painter, const QRectF& location);

        void drawSignalLamp(QPainter* painter, const QRectF& location, const QColor color, const qreal diameter) const;

        QSizeF measureHauptSignal(HauptSignalRenderingStyle style,
                                  const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm) const;
        void drawHauptSignal(QPainter* painter, HauptSignalRenderingStyle style,
                             const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm,
                             const QRectF& location) const;

        QSizeF measureVorSignal(const Openstw::Simulation::VorSignalSchirm* vorSignalSchirm) const;
        void drawVorSignal(QPainter* painter, const Openstw::Simulation::VorSignalSchirm* vorSignalSchirm,
                           const QRectF& location) const;

    protected:
        Openstw::Simulation::TileElementDirection m_direction;
    };
}
