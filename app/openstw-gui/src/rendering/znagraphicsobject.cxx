#include <QPainter>

#include "../application.hxx"
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

        const auto displayState = this->m_tile->zugnummernAnzeige().displayState();

        std::array<int, 6> zugNummerDigits = {1, 2, 3, 4, 5, 6};
        for (int digitIdx = 0; digitIdx < 6; ++digitIdx)
        {
            if (zugNummerDigits[digitIdx] == -1)
                continue;

            if (displayState == Openstw::Simulation::ZugnummernAnzeigeState::AllDigitsBlinking &&
                !this->m_blinkingDigitState)
            {
                break;
            }

            if (digitIdx == 5 && displayState == Openstw::Simulation::ZugnummernAnzeigeState::LastDigitBlinking &&
                !this->m_blinkingDigitState)
            {
                break;
            }

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

    void ZNAGraphicsObject::drawLargeDisplay(QPainter* painter, HorizontalDirection displayPart,
                                             const QString& zugnummer)
    {
        painter->save();

        auto displayRect = this->calculateLargeDisplayRect(displayPart);

        // We need to make the rectangle a bit bigger to cover
        // the separator between the two tiles, and to hide the border.
        // We also need to adjust the clipping rectangle to allows to overdraw.
        if (displayPart == HorizontalDirection::Left)
        {
            displayRect =
                displayRect.marginsAdded(QMarginsF{0.0f, 0.0f, ZNAGraphicsObject::largeDisplayOverdrawAmount, 0.0f});

            auto currentClipRect = painter->clipBoundingRect();
            currentClipRect = currentClipRect.marginsAdded(QMarginsF{0.0f, 0.0f, 4.0f, 0.0f});
            painter->setClipRect(currentClipRect);
        }
        else
        {
            displayRect =
                displayRect.marginsAdded(QMarginsF{ZNAGraphicsObject::largeDisplayOverdrawAmount, 0.0f, 0.0f, 0.0f});

            auto currentClipRect = painter->clipBoundingRect();
            currentClipRect = currentClipRect.marginsAdded(QMarginsF{4.0f, 0.0f, 0.0f, 0.0f});
            painter->setClipRect(currentClipRect);
        }

        painter->setPen(rectanglePen(ZNAGraphicsObject::displayBorderColor, 1.0f));
        painter->setBrush(ZNAGraphicsObject::displayBackgroundColor);
        painter->drawRect(adjustRectForBorder(displayRect, 1.0f));

        // Draw digits
        const auto digitAreaRect = this->calculateLargeDisplayDigitsRect(displayPart);
        const auto digitAreaSize = this->m_largeDigitRenderer.digitAreaSize();

        const auto zugNummerDigits =
            displayPart == HorizontalDirection::Left ? std::array<int, 3>{1, 2, 3} : std::array<int, 3>{4, 5, 6};

        const auto displayState = this->m_tile->zugnummernAnzeige().displayState();

        for (int digitIdx = 0; digitIdx < 3; ++digitIdx)
        {
            if (zugNummerDigits[digitIdx] == -1)
                continue;

            if (displayState == Openstw::Simulation::ZugnummernAnzeigeState::AllDigitsBlinking &&
                !this->m_blinkingDigitState)
            {
                break;
            }

            if (displayPart == HorizontalDirection::Right)
            {
                if (digitIdx == 2 && displayState == Openstw::Simulation::ZugnummernAnzeigeState::LastDigitBlinking &&
                    !this->m_blinkingDigitState)
                {
                    break;
                }
            }

            const QRectF digitRect{digitAreaRect.left() + (digitIdx * (digitAreaSize.width() +
                                                                       ZNAGraphicsObject::largeDisplayDigitPadding)),
                                   digitAreaRect.top(), digitAreaSize.width(), digitAreaSize.height()};

            this->m_largeDigitRenderer.drawDigit(painter, digitRect, ZNAGraphicsObject::displayDigitColor,
                                                 zugNummerDigits[digitIdx]);
        }

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

    QRectF ZNAGraphicsObject::calculateLargeDisplayRect(HorizontalDirection displayPart) const
    {
        const auto boundingRect = this->boundingRect();

        const qreal displayWidth = (boundingRect.width() - ZNAGraphicsObject::largeDisplaySidePadding);

        // We overdraw the rect by a few pixels so that the rectangle border isnt visible.
        if (displayPart == HorizontalDirection::Left)
        {
            return QRectF{
                boundingRect.left() + ZNAGraphicsObject::largeDisplaySidePadding,
                centerWithin(ZNAGraphicsObject::largeDisplayHeight, boundingRect.height(), boundingRect.top()),
                displayWidth, ZNAGraphicsObject::largeDisplayHeight};
        }
        else
        {
            return QRectF{
                boundingRect.left(),
                centerWithin(ZNAGraphicsObject::largeDisplayHeight, boundingRect.height(), boundingRect.top()),
                displayWidth, ZNAGraphicsObject::largeDisplayHeight};
        }
    }

    QRectF ZNAGraphicsObject::calculateLargeDisplayDigitsRect(HorizontalDirection displayPart) const
    {
        const auto displayRect = this->calculateLargeDisplayRect(displayPart);

        const auto margins = (displayPart == HorizontalDirection::Left)
                                 ? QMarginsF{2.0f, 4.0f, ZNAGraphicsObject::largeDisplayDigitPadding / 2.0f, 4.0f}
                                 : QMarginsF{ZNAGraphicsObject::largeDisplayDigitPadding / 2.0f, 4.0f, 2.0f, 4.0f};

        return displayRect.marginsRemoved(margins);
    }

    void ZNAGraphicsObject::timerTick()
    {
        this->m_blinkingDigitState = !this->m_blinkingDigitState;

        // We have to invalidate ourselves if our display wants to display its digits
        if (this->m_tile->hasZugnummernAnzeige())
        {
            const auto displayState = this->m_tile->zugnummernAnzeige().displayState();

            if (displayState != Openstw::Simulation::ZugnummernAnzeigeState::Static)
            {
                this->update();
            }
        }
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
        else
        {
            const auto displayPart = (displayType == Openstw::Simulation::ZugnummernAnzeigeType::LargeLeftPart)
                                         ? HorizontalDirection::Left
                                         : HorizontalDirection::Right;

            this->drawLargeDisplay(painter, displayPart, zna.currentZugNummer());
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

        this->m_compactDigitRenderer.setup(digitAreaSize, ZNAGraphicsObject::compactDisplaySegmentThickness);
        // ==

        // == Calculate digit size for large-style seven segment display, and setup
        // its renderer
        const auto largeDigitAreaRect =
            this->calculateLargeDisplayDigitsRect(/*Side doesnt matter here.*/ HorizontalDirection::Left);

        const QSizeF largeDigitAreaSize{
            (largeDigitAreaRect.width() - 2.0f * ZNAGraphicsObject::largeDisplayDigitPadding) / 3.0f,
            largeDigitAreaRect.height()};

        this->m_largeDigitRenderer.setup(largeDigitAreaSize, ZNAGraphicsObject::largeDisplaySegmentThickness);
        // ==

        connect(gApplication->timer2Hz(), &QTimer::timeout, this, &ZNAGraphicsObject::timerTick);
    }
}
