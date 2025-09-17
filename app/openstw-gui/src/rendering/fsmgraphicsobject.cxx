#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "fsmgraphicsobject.hxx"
#include "renderinghelpers.hxx"

namespace Rendering
{

    FSMGraphicsObject::FSMGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent, false)
    {
    }

    QRectF FSMGraphicsObject::boundingRect() const
    {
        const auto tileBoundingRect = this->tileGraphicsObject()->innerBoundingRect();

        if (!this->m_tile->hasFeststellMelder())
            return QRectF{};

        const auto& feststellMelder = this->m_tile->feststellMelder();
        const auto direction = feststellMelder.direction();

        if (direction == Openstw::Simulation::TileElementDirection::Forward)
        {
            // Aligned to right edge
            const auto drawingAreaRect = this->tileGraphicsObject()->horizontalDrawingAreaRect(VerticalDirection::Top);

            return QRectF{drawingAreaRect.right() - FSMGraphicsObject::feststellMelderPaddingToBorder -
                              FSMGraphicsObject::feststellMelderSideLength,
                          centerWithin(FSMGraphicsObject::feststellMelderSideLength, drawingAreaRect.height(),
                                       drawingAreaRect.top()),
                          FSMGraphicsObject::feststellMelderSideLength, FSMGraphicsObject::feststellMelderSideLength};
        }
        else if (direction == Openstw::Simulation::TileElementDirection::Backward)
        {
            // Aligned to left edge
            const auto drawingAreaRect =
                this->tileGraphicsObject()->horizontalDrawingAreaRect(VerticalDirection::Bottom);

            return QRectF{drawingAreaRect.left() + FSMGraphicsObject::feststellMelderPaddingToBorder,
                          centerWithin(FSMGraphicsObject::feststellMelderSideLength, drawingAreaRect.height(),
                                       drawingAreaRect.top()),
                          FSMGraphicsObject::feststellMelderSideLength, FSMGraphicsObject::feststellMelderSideLength};
        }
        else
        {     
            // Centered
            const auto drawingAreaRect = this->tileGraphicsObject()->horizontalDrawingAreaRect(VerticalDirection::Top);

            return QRectF{centerWithin(FSMGraphicsObject::feststellMelderSideLength, drawingAreaRect.width(),
                                       drawingAreaRect.left()),
                          centerWithin(FSMGraphicsObject::feststellMelderSideLength, drawingAreaRect.height(),
                                       drawingAreaRect.top()),
                          FSMGraphicsObject::feststellMelderSideLength, FSMGraphicsObject::feststellMelderSideLength};
        }
    }

    void FSMGraphicsObject::positionSelf()
    {
        this->setPos(this->tileGraphicsObject()->innerBoundingRect().topLeft());
    }

    void FSMGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasFeststellMelder())
            return;

        const auto state = this->m_tile->feststellMelder().state();

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();

        painter->save();

        const auto feststellMelderInnerColor = (state == Openstw::Simulation::StaticLampState::Off)
                                                   ? FSMGraphicsObject::feststellMelderInactiveColor
                                                   : FSMGraphicsObject::feststellMelderActiveColor;

        painter->setPen(rectanglePen(Qt::black, FSMGraphicsObject::feststellMelderBorderThickness));
        painter->setBrush(feststellMelderInnerColor);

        painter->drawRect(adjustRectForBorder(boundingRect, FSMGraphicsObject::feststellMelderBorderThickness));

        painter->restore();
    }
}

