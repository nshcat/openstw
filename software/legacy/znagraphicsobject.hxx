#pragma once

#include "renderinghelpers.hxx"
#include "sevensegmentrenderer.hxx"
#include "tilecomponentgraphicsobject.hxx"
#include <QObject>
#include <array>

class TileGraphicsObject;

namespace Rendering
{
    class ZNAGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal compactDisplayHeight = 32.0f;
        constexpr static qreal compactDisplaySidePadding = 4.0f;

        constexpr static QColor displayBackgroundColor{0x25, 0x25, 0x25};
        constexpr static QColor displayBorderColor{0x3f, 0x3f, 0x3f};
        constexpr static QColor displayDigitColor{0xf9, 0x6f, 0x71};

        constexpr static qreal compactDisplayDigitPadding = 2.0f;
        constexpr static qreal compactDisplayLabelWidth = 120.0f;
        constexpr static qreal compactDisplayLabelHeight = 26.5f;
        constexpr static qreal compactDisplayLabelBottomPadding = 2.0f;
        constexpr static qreal compactDisplaySegmentThickness = 2.0f;

        constexpr static qreal largeDisplaySidePadding = 40.0f;
        constexpr static qreal largeDisplayHeight = 50.0f;
        constexpr static qreal largeDisplaySegmentThickness = 3.0f;
        constexpr static qreal largeDisplayDigitPadding = 6.0f;
        constexpr static qreal largeDisplayOverdrawAmount = 5.0f;

    public:
        ZNAGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;
        virtual void afterSetup() override;

    protected:
        void drawCompactDisplay(QPainter* painter, const Openstw::Simulation::ZugnummernAnzeigeState displayState,
                                const Openstw::Simulation::Zugnummer&, const QString& label) const;
        void drawCompactLabel(QPainter* painter, const QString& label) const;

        void drawLargeDisplay(QPainter* painter, const Openstw::Simulation::ZugnummernAnzeigeState displayState,
                              HorizontalDirection displayPart, const Openstw::Simulation::Zugnummer& zugnummer);

        QRectF calculateCompactDisplayRect() const;
        QRectF calculateCompactDisplayDigitsRect() const;

        QRectF calculateLargeDisplayRect(HorizontalDirection displayPart) const;
        QRectF calculateLargeDisplayDigitsRect(HorizontalDirection displayPart) const;

    protected slots:
        void timerTick();

    protected:
        SevenSegmentRenderer m_compactDigitRenderer{};
        SevenSegmentRenderer m_largeDigitRenderer{};
        bool m_blinkingDigitState{false};
    };
}
