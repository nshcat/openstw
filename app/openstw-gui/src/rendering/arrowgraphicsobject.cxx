#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "arrowgraphicsobject.hxx"
#include "renderinghelpers.hxx"

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

    void ArrowGraphicsObject::drawLabel(QPainter* painter, LabelBoxLocation location, const QString& label) const
    {
        const auto boundingRect = this->tileGraphicsObject()->innerBoundingRect();

        const qreal labelBoxYPos = (location == LabelBoxLocation::Top)
                                       ? (boundingRect.top() + ArrowGraphicsObject::labelBoxBorderPadding)
                                       : (boundingRect.bottom() - ArrowGraphicsObject::labelBoxBorderPadding -
                                          ArrowGraphicsObject::labelBoxHeight);

        const QRectF labelBoxRect{
            centerWithin(ArrowGraphicsObject::labelBoxWidth, boundingRect.width(), boundingRect.left()), labelBoxYPos,
            ArrowGraphicsObject::labelBoxWidth, ArrowGraphicsObject::labelBoxHeight};

        drawTextBox(painter, labelBoxRect, label, Qt::white, Qt::transparent, 0.0f, Qt::black);
    }

    void ArrowGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasDirectionArrows())
            return;

        const auto& arrows = this->m_tile->directionArrows();

        const auto arrowDirections = arrows.directions();
        if (!arrowDirections.hasAny())
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

        bool hasLeftArrow = arrowDirections.has(Openstw::Simulation::ArrowDirection::Left);
        bool hasRightArrow = arrowDirections.has(Openstw::Simulation::ArrowDirection::Right);
        LabelBoxLocation labelLocation{LabelBoxLocation::Top};

        // If both arrows are to be drawn, the alignment doesnt matter. The arrows point towards the center.
        if (hasLeftArrow && hasRightArrow)
        {
            leftHalfHasArrow = true;
            leftHalfArrowDirection = Openstw::Simulation::ArrowDirection::Right;
            rightHalfHasArrow = true;
            rightHalfArrowDirection = Openstw::Simulation::ArrowDirection::Left;
            labelLocation = LabelBoxLocation::Bottom;
        }
        else if (hasLeftArrow)
        {
            if (arrows.alignment() == Openstw::Simulation::ArrowAlignment::Left)
            {
                leftHalfHasArrow = true;
                leftHalfArrowDirection = Openstw::Simulation::ArrowDirection::Left;
            }
            else
            {
                rightHalfHasArrow = true;
                rightHalfArrowDirection = Openstw::Simulation::ArrowDirection::Left;
            }

            labelLocation = LabelBoxLocation::Top;
        }
        else if (hasRightArrow)
        {
            if (arrows.alignment() == Openstw::Simulation::ArrowAlignment::Left)
            {
                leftHalfHasArrow = true;
                leftHalfArrowDirection = Openstw::Simulation::ArrowDirection::Right;
            }
            else
            {
                rightHalfHasArrow = true;
                rightHalfArrowDirection = Openstw::Simulation::ArrowDirection::Right;
            }

            labelLocation = LabelBoxLocation::Bottom;
        }

        if (leftHalfHasArrow)
            this->drawArrow(painter, leftHalfArrowDirection, leftHalf);
        if (rightHalfHasArrow)
            this->drawArrow(painter, rightHalfArrowDirection, rightHalf);

        if (arrows.hasLabel())
            this->drawLabel(painter, labelLocation, arrows.label());
    }
}
