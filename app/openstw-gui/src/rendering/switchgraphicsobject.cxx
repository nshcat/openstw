#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "switchgraphicsobject.hxx"
#include "trackgraphicsobject.hxx"

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

    void SwitchGraphicsObject::drawSimpleSwitch(QPainter* painter, const Openstw::Simulation::SimpleSwitch* sw) const
    {
        painter->save();

        const auto boundingRect = this->boundingRect();
        auto switchDirection = sw->direction();
        const auto branchDirection = sw->branchDirection();

        // We draw everything as if the branch direction is always up,
        // and rotate the drawing by 180 dregrees if the branch direction is actually
        // down.
        bool rotated = false;
        if (branchDirection == Openstw::Simulation::SwitchBranchDirection::Down)
        {
            rotated = true;

            // We have to reverse the direction such that it still fits after rotation.
            if (switchDirection == Openstw::Simulation::TileElementDirection::Forward)
                switchDirection = Openstw::Simulation::TileElementDirection::Backward;
            else
                switchDirection = Openstw::Simulation::TileElementDirection::Forward;

            // Now apply rotation
            painter->translate(boundingRect.center());
            painter->rotate(180.0);
            painter->translate(-boundingRect.center());
        }

        // First, track background for the horizontal track. This is always the same, no
        // matter the switch orientation
        this->drawHorizontalTrackBase(painter, boundingRect);

        this->drawStemIndicator(painter, boundingRect, switchDirection,
                                sw->partState(Openstw::Simulation::SimpleSwitchPart::Stem));

        this->drawStraightIndicator(painter, boundingRect, switchDirection,
                                    sw->partState(Openstw::Simulation::SimpleSwitchPart::Straight));

        this->drawPivot(painter, boundingRect, switchDirection);

        painter->restore();
    }

    void SwitchGraphicsObject::drawHorizontalTrackBase(QPainter* painter, const QRectF& boundingRect) const
    {
        painter->save();

        const QRectF trackRect{
            boundingRect.left(),
            centerWithin(TrackGraphicsObject::trackThickness, boundingRect.height(), boundingRect.top()),
            boundingRect.width(), TrackGraphicsObject::trackThickness};

        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->setBrush(Qt::black);
        painter->drawRect(adjustRectForBorder(trackRect, 1.0f));

        painter->restore();
    }

    void SwitchGraphicsObject::drawPivot(QPainter* painter, const QRectF& boundingRect,
                                         Openstw::Simulation::TileElementDirection direction) const
    {
        painter->save();

        const qreal pivotXPos = (direction == Openstw::Simulation::TileElementDirection::Forward)
                                    ? boundingRect.left() + SwitchGraphicsObject::pivotToBorderPadding
                                    : boundingRect.right() - SwitchGraphicsObject::pivotToBorderPadding -
                                          SwitchGraphicsObject::pivotDiameter;

        const QRectF pivotRect{
            pivotXPos, centerWithin(SwitchGraphicsObject::pivotDiameter, boundingRect.height(), boundingRect.top()),
            SwitchGraphicsObject::pivotDiameter, SwitchGraphicsObject::pivotDiameter};

        painter->setPen(QPen{SwitchGraphicsObject::pivotOutlineColor, 1.0f});
        painter->setBrush(Qt::black);
        painter->drawEllipse(pivotRect);

        painter->restore();
    }

    void SwitchGraphicsObject::drawStemIndicator(QPainter* painter, const QRectF& boundingRect,
                                                 Openstw::Simulation::TileElementDirection direction,
                                                 Openstw::Simulation::TrackState state) const
    {
        painter->save();

        const auto indicatorColor = TrackGraphicsObject::colorForTrackState(state);

        const qreal indicatorXPos = (direction == Openstw::Simulation::TileElementDirection::Forward)
                                        ? boundingRect.left() + SwitchGraphicsObject::stemIndicatorToBorderPadding
                                        : boundingRect.right() - SwitchGraphicsObject::stemIndicatorToBorderPadding -
                                              SwitchGraphicsObject::stemIndicatorWidth;

        const QRectF indicatorRect{
            indicatorXPos,
            centerWithin(TrackGraphicsObject::trackIndicatorHeight, boundingRect.height(), boundingRect.top()),
            SwitchGraphicsObject::stemIndicatorWidth, TrackGraphicsObject::trackIndicatorHeight};

        painter->setPen(rectanglePen(indicatorColor, 1.0f));
        painter->setBrush(indicatorColor);
        painter->drawRect(adjustRectForBorder(indicatorRect, 1.0f));

        painter->restore();
    }

    void SwitchGraphicsObject::drawStraightIndicator(QPainter* painter, const QRectF& boundingRect,
                                                     Openstw::Simulation::TileElementDirection direction,
                                                     Openstw::Simulation::TrackState state) const
    {
        painter->save();

        const auto indicatorColor = TrackGraphicsObject::colorForTrackState(state);

        const qreal indicatorXPos = (direction == Openstw::Simulation::TileElementDirection::Forward)
                                        ? boundingRect.right() -
                                              SwitchGraphicsObject::straightIndicatorToBorderPadding -
                                              SwitchGraphicsObject::straightIndicatorWidth
                                        : boundingRect.left() + SwitchGraphicsObject::straightIndicatorToBorderPadding;

        const QRectF indicatorRect{
            indicatorXPos,
            centerWithin(TrackGraphicsObject::trackIndicatorHeight, boundingRect.height(), boundingRect.top()),
            SwitchGraphicsObject::straightIndicatorWidth, TrackGraphicsObject::trackIndicatorHeight};

        painter->setPen(rectanglePen(indicatorColor, 1.0f));
        painter->setBrush(indicatorColor);
        painter->drawRect(adjustRectForBorder(indicatorRect, 1.0f));

        painter->restore();
    }

    void SwitchGraphicsObject::drawBranch(QPainter* painter, const QRectF& boundingRect,
                                          Openstw::Simulation::TileElementDirection direction,
                                          Openstw::Simulation::TrackState state) const
    {
    }
}
