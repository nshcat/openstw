#pragma once

#include "renderinghelpers.hxx"
#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    class ErlaubsnisFeldGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal erlaubnisMelderHeight = 15.0f;
        constexpr static qreal erlaubnisMelderWidth = 45.0f;
        constexpr static qreal textBoxHeight = 14.0f;
        constexpr static qreal textBoxWidth = 38.0f;

        constexpr static QColor activeErlaubnisMelderColor{0xFE, 0xFF, 0x7F};
        constexpr static QColor inactiveErlaubnisMelderColor{0x23, 0x23, 0x23};

        constexpr static qreal buttonDiameter = 25.0f;
        constexpr static QColor buttonInnerColor{0xC8, 0xC8, 0xC8};
        constexpr static QColor buttonOuterColor{0x74, 0x74, 0x74};

        constexpr static qreal horzPaddingToBorder = 10.0f;
        constexpr static qreal buttonLabelPadding = 5.0f;

        constexpr static qreal ausfahrSperrLampDiameter = 11.0f;
        constexpr static QColor inactiveAusfahrSperrLampColor{0x11, 0x1e, 0x3a};
        constexpr static QColor activeAusfahrSperrLampColor{0x0, 0x0, 0xFF};
        constexpr static qreal ausfahrSperrLampPadding = 10.0f;

    public:
        ErlaubsnisFeldGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        QRectF calculateHemiRect(const VerticalDirection location) const;

        void drawErlaubnisMelder(QPainter* painter, const QRectF& location, QColor melderColor,
                                 const HorizontalDirection direction) const;

        void drawButton(QPainter* painter, const QRectF& location) const;
        void drawEaTLabel(QPainter* painter, const QRectF& location, const bool rotated) const;
        void drawAusfahrLabel(QPainter* painter, const QRectF& location, const QString& text, const bool rotated) const;
        void drawAusfahrSperrMelder(QPainter* painter, const QRectF& location, bool lampOn);
    };
}
