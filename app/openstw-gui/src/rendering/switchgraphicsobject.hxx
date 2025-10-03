#pragma once

#include "simpleswitch.hxx"
#include "tilecomponentgraphicsobject.hxx"
#include <QObject>

class TileGraphicsObject;

namespace Rendering
{
    class SwitchGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

    public:
        constexpr static qreal pivotToBorderPadding = 45.0f;
        constexpr static qreal pivotDiameter = 17.0f;
        constexpr static QColor pivotOutlineColor{0x4b, 0x4b, 0x4b};

        constexpr static qreal stemIndicatorWidth = 20.0f;
        constexpr static qreal stemIndicatorToBorderPadding = 16.0f;

        constexpr static qreal straightIndicatorWidth = 37.0f;
        constexpr static qreal straightIndicatorToBorderPadding = 49.0f;

        constexpr static qreal branchIndicatorToPivotPadding = 10.0f;
        constexpr static qreal branchIndicatorLength = 21.0f;

        constexpr static qreal branchAngle = 27.0;

    public:
        SwitchGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    public:
        static QRectF CalcSwitchButtonLocation(const QRectF& boundingRect,
                                               Openstw::Simulation::TileElementDirection direction);

    protected:
        virtual void positionSelf() override;

    protected:
        void drawSimpleSwitch(QPainter* painter, const Openstw::Simulation::SimpleSwitch* sw) const;

        void drawHorizontalTrackBase(QPainter* painter, const QRectF& boundingRect) const;
        void drawPivot(QPainter* painter, const QRectF& boundingRect,
                       Openstw::Simulation::TileElementDirection direction) const;
        void drawStemIndicator(QPainter* painter, const QRectF& boundingRect,
                               Openstw::Simulation::TileElementDirection direction,
                               Openstw::Simulation::TrackState state) const;
        void drawStraightIndicator(QPainter* painter, const QRectF& boundingRect,
                                   Openstw::Simulation::TileElementDirection direction,
                                   Openstw::Simulation::TrackState state) const;
        void drawBranch(QPainter* painter, const QRectF& boundingRect, const QRectF& buttonRect,
                        Openstw::Simulation::TileElementDirection direction,
                        Openstw::Simulation::TrackState state) const;
    };
}
