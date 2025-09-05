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

        const qreal displayRectWidth = boundingRect.width() - 2.0f * ZNAGraphicsObject::compactDisplaySidePadding;

        const QRectF displayRect{
            boundingRect.left() + ZNAGraphicsObject::compactDisplaySidePadding,
            centerWithin(ZNAGraphicsObject::compactDisplayHeight, boundingRect.height(), boundingRect.top()),
            displayRectWidth, ZNAGraphicsObject::compactDisplayHeight};

        // Draw background
        painter->setPen(rectanglePen(ZNAGraphicsObject::displayBorderColor, 1.0f));
        painter->setBrush(ZNAGraphicsObject::displayBackgroundColor);
        painter->drawRect(adjustRectForBorder(displayRect, 1.0f));

        // Draw digits
        const auto digitAreaRect = displayRect.marginsRemoved(QMarginsF{1.0f, 2.0f, 1.0f, 2.0f});
        const QSizeF digitSize{(digitAreaRect.width() - 5.0f * ZNAGraphicsObject::compactDisplayDigitPadding) / 6.0f,
                               digitAreaRect.height()};

        std::array<int, 6> zugNummerDigits = {1, 2, 3, 4, 5, 6};
        for (int digitIdx = 0; digitIdx < 6; ++digitIdx)
        {
            if (zugNummerDigits[digitIdx] == -1)
                continue;

            const QRectF digitRect{digitAreaRect.left() +
                                       (digitIdx * (digitSize.width() + ZNAGraphicsObject::compactDisplayDigitPadding)),
                                   digitAreaRect.top(), digitSize.width(), digitSize.height()};

            this->drawDigit(painter, digitRect, 2.0f, zugNummerDigits[digitIdx]);
        }

        painter->restore();
    }

    void ZNAGraphicsObject::drawDigit(QPainter* painter, const QRectF& location, const qreal segmentThickness,
                                      const int digit) const
    {
        painter->save();

        if (digit < 0 || digit > 9)
            throw std::runtime_error("Digit out of range");

        const QSizeF digitSize{location.width() - 8, location.height()};
        const QRectF digitBounds = {centerWithin(digitSize.width(), location.width(), location.left()),
                                    centerWithin(digitSize.height(), location.height(), location.top()),
                                    digitSize.width(), digitSize.height()};

        painter->setPen(rectanglePen(ZNAGraphicsObject::displayDigitColor, 0.5f));
        painter->setBrush(ZNAGraphicsObject::displayDigitColor);

        painter->translate(digitBounds.center());
        painter->shear(-0.1, 0.0);
        painter->translate(-digitBounds.center());
        painter->translate(digitBounds.topLeft());

        const auto segmentStates = ZNAGraphicsObject::digitSegments[digit];
        for (int i = 0; i < 7; ++i)
        {
            const auto segmentPolygon = this->calculateSegmentPolygon(digitBounds.size(), segmentThickness, i);

            if ((segmentStates >> i) & 1)
                painter->drawPolygon(segmentPolygon);
        }

        painter->restore();
    }

    QPolygonF ZNAGraphicsObject::calculateSegmentPolygon(const QSizeF digitSize, const qreal segmentThickness,
                                                         const int segmentNr) const
    {
        if (segmentNr < 0 || segmentNr >= 7)
            throw std::runtime_error("Invalid segment number");

        /* clang-format off */
        /**
         *    0
         *    -
         * 5| 6 | 1
         *    -
         * 4| 3 | 2
         *    -
         */

        const qreal halfHeight = digitSize.height() / 2.0f;
        const std::array<QPointF, 7> segmentOffsets{
            QPointF{0.0f, 0.0f},
            QPointF{digitSize.width() - segmentThickness, 0.0f},
            QPointF{digitSize.width() - segmentThickness, halfHeight},
            QPointF{0.0f, digitSize.height() - segmentThickness},
            QPointF{0.0f, halfHeight},
            QPointF{0.0f, 0.0f},
            QPointF{0.0f, halfHeight - segmentThickness/2.0f}
        };
        /* clang-format on */

        const auto segmentOrientation = ZNAGraphicsObject::segmentOrientations[segmentNr];
        QPolygonF baseSegmentPolygon{};
        if (segmentOrientation == SegmentOrientation::Vertical)
            baseSegmentPolygon = this->calculateVerticalSegmentPolygon(digitSize, segmentThickness);
        else
            baseSegmentPolygon = this->calculateHorizontalSegmentPolygon(digitSize, segmentThickness);

        const QPolygonF segmentPolygon = baseSegmentPolygon.translated(segmentOffsets[segmentNr]);

        return segmentPolygon;
    }

    QPolygonF ZNAGraphicsObject::calculateHorizontalSegmentPolygon(const QSizeF digitSize,
                                                                   const qreal segmentThickness) const
    {
        const qreal halfThickness = segmentThickness / 2.0f;

        QPolygonF segment{};

        segment << QPointF{halfThickness, halfThickness} << QPointF{2 * halfThickness, 0.0f}
                << QPointF{digitSize.width() - 2 * halfThickness, 0.0}
                << QPointF{digitSize.width() - halfThickness, halfThickness}
                << QPointF{digitSize.width() - 2 * halfThickness, segmentThickness}
                << QPointF{2 * halfThickness, segmentThickness} << QPointF{halfThickness, halfThickness};

        return segment;
    }

    QPolygonF ZNAGraphicsObject::calculateVerticalSegmentPolygon(const QSizeF digitSize,
                                                                 const qreal segmentThickness) const
    {
        const qreal halfThickness = segmentThickness / 2.0f;

        QPolygonF segment{};

        segment << QPointF{halfThickness, halfThickness} << QPointF{segmentThickness, 2 * halfThickness}
                << QPointF{segmentThickness, digitSize.height() / 2.0f - 2 * halfThickness}
                << QPointF{halfThickness, digitSize.height() / 2.0f - halfThickness}
                << QPointF{0.0, digitSize.height() / 2.0f - 2 * halfThickness} << QPointF{0.0, 2 * halfThickness}
                << QPointF{halfThickness, halfThickness};

        return segment;
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
        }
    }
}
