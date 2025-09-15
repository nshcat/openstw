#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "zstgraphicsobject.hxx"

namespace Rendering
{

    ZSTGraphicsObject::ZSTGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent, true)
    {
    }

    QRectF ZSTGraphicsObject::boundingRect() const
    {
        const auto tileBoundingRect = this->tileGraphicsObject()->innerBoundingRect();

        if (!this->m_tile->hasZugStrassenTaste())
            return QRectF{};

        const auto direction = this->m_tile->zugStrassenTasteDirection();
        if (direction == Openstw::Simulation::TileElementDirection::Forward)
        {
            // Aligned to left edge
            return QRectF{
                tileBoundingRect.right() - ZSTGraphicsObject::buttonToBorderPadding -
                    ZSTGraphicsObject::buttonOuterDiameter,
                centerWithin(ZSTGraphicsObject::buttonOuterDiameter, tileBoundingRect.height(), tileBoundingRect.top()),
                ZSTGraphicsObject::buttonOuterDiameter, ZSTGraphicsObject::buttonOuterDiameter};
        }
        else if (direction == Openstw::Simulation::TileElementDirection::Backward)
        {
            // Aligned to right edge
            return QRectF{
                tileBoundingRect.left() + ZSTGraphicsObject::buttonToBorderPadding,
                centerWithin(ZSTGraphicsObject::buttonOuterDiameter, tileBoundingRect.height(), tileBoundingRect.top()),
                ZSTGraphicsObject::buttonOuterDiameter, ZSTGraphicsObject::buttonOuterDiameter};
        }
        else
        {
            // Centered
            return QRectF{
                centerWithin(ZSTGraphicsObject::buttonOuterDiameter, tileBoundingRect.width(), tileBoundingRect.left()),
                centerWithin(ZSTGraphicsObject::buttonOuterDiameter, tileBoundingRect.height(), tileBoundingRect.top()),
                ZSTGraphicsObject::buttonOuterDiameter, ZSTGraphicsObject::buttonOuterDiameter};
        }
    }

    void ZSTGraphicsObject::positionSelf()
    {
        this->setPos(this->tileGraphicsObject()->innerBoundingRect().topLeft());
    }

    void ZSTGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasZugStrassenTaste())
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();

        painter->save();

        const QRectF outerButtonRect{
            centerWithin(ZSTGraphicsObject::buttonOuterDiameter, boundingRect.width(), boundingRect.left()),
            centerWithin(ZSTGraphicsObject::buttonOuterDiameter, boundingRect.height(), boundingRect.top()),
            ZSTGraphicsObject::buttonOuterDiameter, ZSTGraphicsObject::buttonOuterDiameter};

        painter->setPen(QPen{ZSTGraphicsObject::buttonBorderColor, 0.5f});
        painter->setBrush(ZSTGraphicsObject::buttonOuterColor);
        painter->drawEllipse(outerButtonRect);

        const QRectF innerButtonRect{
            centerWithin(ZSTGraphicsObject::buttonInnerDiameter, boundingRect.width(), boundingRect.left()),
            centerWithin(ZSTGraphicsObject::buttonInnerDiameter, boundingRect.height(), boundingRect.top()),
            ZSTGraphicsObject::buttonInnerDiameter, ZSTGraphicsObject::buttonInnerDiameter};

        painter->setPen(QPen{ZSTGraphicsObject::buttonBorderColor, 0.5f});
        painter->setBrush(ZSTGraphicsObject::buttonInnerColor);
        painter->drawEllipse(innerButtonRect);

        painter->restore();
    }

    std::optional<Qt::MouseButtons> ZSTGraphicsObject::acceptsMouseInput() const
    {
        if (this->m_tile->hasZugStrassenTaste())
        {
            return {Qt::MouseButton::LeftButton | Qt::MouseButton::RightButton};
        }
        else
        {
            return std::nullopt;
        }
    }

    void ZSTGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        if (event->button() & Qt::MouseButton::LeftButton)
        {
            qDebug("Left click on Zugstreckentaste");
        }
    }

    void ZSTGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
    {
        QMenu contextMenu{event->widget()};
        contextMenu.addSection("Zugstraßentaste");
        contextMenu.addAction("Help");

        contextMenu.exec(event->screenPos());
    }
}

