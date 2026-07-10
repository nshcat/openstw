#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "labelgraphicsobject.hxx"
#include "renderinghelpers.hxx"

namespace Rendering
{

    LabelGraphicsObject::LabelGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF LabelGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void LabelGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void LabelGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasTileLabel())
            return;

        const auto& label = this->m_tile->tileLabel();

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();

        painter->save();

        const qreal labelWidth = (boundingRect.width() - 2.0f * LabelGraphicsObject::labelBoxHorzPadding);
        const qreal labelHeight = (boundingRect.height() - 2.0f * LabelGraphicsObject::labelBoxVertPadding);

        const QRectF labelRect{centerWithin(labelWidth, boundingRect.width(), boundingRect.left()),
                               centerWithin(labelHeight, boundingRect.height(), boundingRect.top()), labelWidth,
                               labelHeight};

        drawTextBox(painter, labelRect, label, Qt::white, Qt::transparent, 0.0f, Qt::black);

        painter->restore();
    }
}
