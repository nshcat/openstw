#pragma once

#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    /**
     * @brief Graphics object that renders a Feststellmelder
     */
    class FSMGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal feststellMelderSideLength = 18.0f;
        constexpr static qreal feststellMelderBorderThickness = 2.0f;
        constexpr static QColor feststellMelderInactiveColor{0x23, 0x23, 0x23};
        constexpr static QColor feststellMelderActiveColor{0xec, 0xfe, 0xbf};
        constexpr static qreal feststellMelderPaddingToBorder = 15.5f;

    public:
        FSMGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;
    };
}
