#pragma once

#include <QObject>
#include <optional>

#include "kennlichtstyle.hxx"
#include "signalpainter.hxx"

namespace Rendering
{
    // Painter that renders a single Hauptsignalschirm.
    class HauptSignalSchirmPainter : public SignalPainter
    {
        Q_OBJECT

    protected: // Drawing constants
        constexpr static qreal signalHeight = 18.0f;
        constexpr static qreal signalLampPadding = 2.5f;
        constexpr static qreal signalToBorderPadding = 20.0f;
        constexpr static QColor kennLichtColor{0xFE, 0xFF, 0x7F};
        constexpr static QColor inactiveLampColor{0x23, 0x23, 0x23};
        constexpr static qreal inactiveLampDiameter = 11.0f;
        constexpr static qreal activeLampDiameter = 14.0f;
        constexpr static qreal activeKennLampDiameter = 11.0f;
        constexpr static qreal inactiveKennLampDiameter = 8.0f;
        constexpr static qreal hpSpaceForLamp = activeLampDiameter;
        constexpr static qreal hpPaddingBetweenLamps = 0.66f * hpSpaceForLamp;

    public:
        explicit HauptSignalSchirmPainter(Aperture* redLamp, Aperture* greenLamp, BlinkGenerator* blink,
                                          QObject* parent = nullptr);

    public:
        // Aperture for the green signal lamp
        const Aperture* greenLamp() const;

        // Aperture for the red signal lamp
        const Aperture* redLamp() const;

        // Optional aperture for the Sh1 lamp
        std::optional<const Aperture*> shuntingLamp() const;

        // Whether the renderer will draw a shunting signal lamp.
        bool hasShuntingLamp() const;

        // Optional aperture for the Kennlicht
        std::optional<const Aperture*> kennlichtLamp() const;

        // Whether the renderer will draw a kennlicht lamp.
        bool hasKennlicht() const;

        // How to render the Kennlicht, if there is one
        KennlichtStyle kennlichtStyle() const;

    public:
        // Set aperture for the Sh1 lamp
        void setShuntingLamp(const Aperture* newShuntingLamp);

        // Set aperture for the Kennlicht
        void setKennlichtLamp(const Aperture* newKennlichtLamp);

        // Specify how to render the Kennlicht, if there is one
        void setKennlichtStyle(KennlichtStyle newKennlichtStyle);

    public:
        // Measure out Hauptsignalschirm.
        virtual QSizeF measure() override;

        virtual void paint(QPainter* painter, const QRectF& where) override;

    protected:
        // Aperture for the green signal lamp
        const Aperture* m_greenLamp;

        // Aperture for the red signal lamp
        const Aperture* m_redLamp;

        // Optional aperture for the Sh1 lamp.
        std::optional<const Aperture*> m_shuntingLamp{};

        // Optional aperture for the Kennlicht.
        std::optional<const Aperture*> m_kennlichtLamp{};

        // How to render the Kennlicht, if there is one.
        KennlichtStyle m_kennlichtStyle{KennlichtStyle::Individual};
    };
}
