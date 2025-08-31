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

    public:
        constexpr static qreal signalToBorderPadding = 20.0f;
        constexpr static QColor kennLichtColor{0xFE, 0xFF, 0x7F};
        constexpr static QColor inactiveLampColor{0x23, 0x23, 0x23};
        constexpr static qreal inactiveLampDiameter = 11.0f;
        constexpr static qreal activeLampDiameter = 14.0f;
        constexpr static qreal signalLampPadding = 2.5f;
        constexpr static qreal activeKennLampDiameter = 11.0f;
        constexpr static qreal inactiveKennLampDiameter = 8.0f;

    public:
        SignalGraphicsObject(TileGraphicsObject* parent, const Openstw::Simulation::TileElementDirection direction);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawSignalLamp(QPainter* painter, const QRectF& location, const QColor color, const qreal diameter) const;

        QSizeF measureHauptSignal(HauptSignalRenderingStyle style,
                                  const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm);
        void drawHauptSignal(QPainter* painter, HauptSignalRenderingStyle style,
                             const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm,
                             const QRectF& location) const;

    protected:
        Openstw::Simulation::TileElementDirection m_direction;
    };

} // namespace Rendering
