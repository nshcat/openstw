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

    public:
        SwitchGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawSimpleSwitch(QPainter* painter, const Openstw::Simulation::SimpleSwitch* sw) const;
    };
}
