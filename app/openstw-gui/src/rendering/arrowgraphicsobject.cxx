#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "arrowgraphicsobject.hxx"

namespace Rendering
{

    ArrowGraphicsObject::ArrowGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF ArrowGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void ArrowGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void ArrowGraphicsObject::drawArrow(QPainter* painter, Openstw::Simulation::ArrowDirection direction,
                                        const QRectF& location) const
    {
        painter->save();

        // We are going to purposefully overdraw, so make sure to clip
        // the excess
        // painter->setClipRect(location);

        if (direction == Openstw::Simulation::ArrowDirection::Left)
        {
            painter->translate(location.center());
            painter->rotate(180.0f);
            painter->translate(-location.center());
        }

        const auto center = location.center();

        QPolygonF arrowPolygon{};
        arrowPolygon << location.topLeft()
                     << QPointF{location.right() + location.width() * ArrowGraphicsObject::arrowTipOverDrawFactor,
                                center.y()}
                     << location.bottomLeft() << center << location.topLeft();

        painter->setPen(Qt::black);
        painter->setBrush(Qt::black);
        painter->drawPolygon(arrowPolygon);

        painter->restore();
    }

    void ArrowGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasArrow())
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();
        const auto center = boundingRect.center();

        const QRectF leftHalf{boundingRect.left(), boundingRect.top(), boundingRect.width() / 2.0f,
                              boundingRect.height()};

        const QRectF rightHalf{center.x(), boundingRect.top(), boundingRect.width() / 2.0f, boundingRect.height()};

        bool leftHalfHasArrow{false};
        Openstw::Simulation::ArrowDirection leftHalfArrowDirection;
        bool rightHalfHasArrow{false};
        Openstw::Simulation::ArrowDirection rightHalfArrowDirection;

        bool hasLeftArrow = this->m_tile->arrows().has(Openstw::Simulation::ArrowDirection::Left);
        bool hasRightArrow = this->m_tile->arrows().has(Openstw::Simulation::ArrowDirection::Right);

        // If both arrows are to be drawn, the alignment doesnt matter. The arrows point towards the center.
        if (hasLeftArrow && hasRightArrow)
        {
            leftHalfHasArrow = true;
            leftHalfArrowDirection = Openstw::Simulation::ArrowDirection::Right;
            rightHalfHasArrow = true;
            rightHalfArrowDirection = Openstw::Simulation::ArrowDirection::Left;
        }
        else if (hasLeftArrow)
        {
            if (this->m_tile->arrowAlignment() == Openstw::Simulation::ArrowAlignment::Left)
            {
                leftHalfHasArrow = true;
                leftHalfArrowDirection = Openstw::Simulation::ArrowDirection::Left;
            }
            else
            {
                rightHalfHasArrow = true;
                rightHalfArrowDirection = Openstw::Simulation::ArrowDirection::Left;
            }
        }
        else if (hasRightArrow)
        {
            if (this->m_tile->arrowAlignment() == Openstw::Simulation::ArrowAlignment::Left)
            {
                leftHalfHasArrow = true;
                leftHalfArrowDirection = Openstw::Simulation::ArrowDirection::Right;
            }
            else
            {
                rightHalfHasArrow = true;
                rightHalfArrowDirection = Openstw::Simulation::ArrowDirection::Right;
            }
        }

        if (leftHalfHasArrow)
            this->drawArrow(painter, leftHalfArrowDirection, leftHalf);
        if (rightHalfHasArrow)
            this->drawArrow(painter, rightHalfArrowDirection, rightHalf);
    }
}
