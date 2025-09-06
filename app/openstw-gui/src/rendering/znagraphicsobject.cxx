#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "znagraphicsobject.hxx"

namespace Rendering
{

    ZNAGraphicsObject::ZNAGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF ZNAGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void ZNAGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void ZNAGraphicsObject::drawCompactDisplay(QPainter* painter, const QString& zugnummer, const QString& label) const
    {
        painter->save();

        const auto boundingRect = this->boundingRect();

        const auto displayRect = this->calculateCompactDisplayRect();

        // Draw background
        painter->setPen(rectanglePen(ZNAGraphicsObject::displayBorderColor, 1.0f));
        painter->setBrush(ZNAGraphicsObject::displayBackgroundColor);
        painter->drawRect(adjustRectForBorder(displayRect, 1.0f));

        // Draw digits
        const auto digitAreaRect = this->calculateCompactDisplayDigitsRect();
        const auto digitAreaSize = this->m_compactDigitRenderer.digitAreaSize();

        std::array<int, 6> zugNummerDigits = {1, 2, 3, 4, 5, 6};
        for (int digitIdx = 0; digitIdx < 6; ++digitIdx)
        {
            if (zugNummerDigits[digitIdx] == -1)
                continue;

            const QRectF digitRect{digitAreaRect.left() + (digitIdx * (digitAreaSize.width() +
                                                                       ZNAGraphicsObject::compactDisplayDigitPadding)),
                                   digitAreaRect.top(), digitAreaSize.width(), digitAreaSize.height()};

            this->m_compactDigitRenderer.drawDigit(painter, digitRect, ZNAGraphicsObject::displayDigitColor,
                                                   zugNummerDigits[digitIdx]);
        }

        painter->restore();
    }

    void ZNAGraphicsObject::drawCompactLabel(QPainter* painter, const QString& label) const
    {
        painter->save();

        const auto boundingRect = this->boundingRect();

        const QRectF labelRect{
            centerWithin(ZNAGraphicsObject::compactDisplayLabelWidth, boundingRect.width(), boundingRect.left()),
            boundingRect.bottom() - ZNAGraphicsObject::compactDisplayLabelBottomPadding -
                ZNAGraphicsObject::compactDisplayLabelHeight,
            ZNAGraphicsObject::compactDisplayLabelWidth, ZNAGraphicsObject::compactDisplayLabelHeight};

        drawTextBox(painter, labelRect, label, Qt::white, Qt::transparent, 0.0, Qt::black);

        painter->restore();
    }

    QRectF ZNAGraphicsObject::calculateCompactDisplayRect() const
    {
        const auto boundingRect = this->boundingRect();

        const qreal displayRectWidth = boundingRect.width() - 2.0f * ZNAGraphicsObject::compactDisplaySidePadding;

        const QRectF displayRect{
            boundingRect.left() + ZNAGraphicsObject::compactDisplaySidePadding,
            centerWithin(ZNAGraphicsObject::compactDisplayHeight, boundingRect.height(), boundingRect.top()),
            displayRectWidth, ZNAGraphicsObject::compactDisplayHeight};

        return displayRect;
    }

    QRectF ZNAGraphicsObject::calculateCompactDisplayDigitsRect() const
    {
        return this->calculateCompactDisplayRect().marginsRemoved(QMarginsF{1.0f, 2.0f, 1.0f, 2.0f});
    }

    void ZNAGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasZugnummernAnzeige())
            return;

        const auto& zna = this->m_tile->zugnummernAnzeige();
        const auto displayType = zna.anzeigeType();

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        if (displayType == Openstw::Simulation::ZugnummernAnzeigeType::Compact)
        {
            this->drawCompactDisplay(painter, zna.currentZugNummer(), zna.label());

            if (zna.hasLabel())
            {
                this->drawCompactLabel(painter, zna.label());
            }
        }
    }

    void ZNAGraphicsObject::afterSetup()
    {
        // == Calculate digit size for compact-style seven segment display, and setup
        // its renderer
        const auto digitAreaRect = this->calculateCompactDisplayDigitsRect();

        const QSizeF digitAreaSize{(digitAreaRect.width() - 5.0f * ZNAGraphicsObject::compactDisplayDigitPadding) /
                                       6.0f,
                                   digitAreaRect.height()};

        this->m_compactDigitRenderer.setup(digitAreaSize, 2.0f);
        // ==
    }
}
