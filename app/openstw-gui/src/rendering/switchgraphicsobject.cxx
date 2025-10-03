#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "switchgraphicsobject.hxx"

namespace Rendering
{

    SwitchGraphicsObject::SwitchGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF SwitchGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void SwitchGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void SwitchGraphicsObject::drawSimpleSwitch(QPainter* painter, const Openstw::Simulation::SimpleSwitch* sw) const
    {
        painter->save();

        const auto boundingRect = this->boundingRect();
        const auto switchDirection = sw->direction();

        painter->restore();
    }

    void SwitchGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasSwitch())
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto* sw = this->m_tile->getSwitch();

        switch (sw->type())
        {
        case Openstw::Simulation::SwitchType::Simple:
            {
                const auto* simpleSwitch = dynamic_cast<const Openstw::Simulation::SimpleSwitch*>(sw);
                if (!simpleSwitch)
                    throw std::runtime_error("Expected switch to be SimpleSwitch, but it isn't");

                this->drawSimpleSwitch(painter, simpleSwitch);
                break;
            }
        default:
            {
                break;
            }
        }
    }
}
