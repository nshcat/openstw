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

    void ZNAGraphicsObject::drawDigit(QPainter* painter, const QRectF& location, const char digit) const
    {
        painter->save();

        const QSizeF digitSize{location.width() - 8, location.height()};
        const QRectF digitBounds = {centerWithin(digitSize.width(), location.width(), location.left()),
                                    centerWithin(digitSize.height(), location.height(), location.top()),
                                    digitSize.width(), digitSize.height()};

        painter->setPen(rectanglePen(ZNAGraphicsObject::displayBackgroundColor, 1.0f));
        painter->setBrush(ZNAGraphicsObject::displayBackgroundColor);
        painter->drawRect(adjustRectForBorder(location, 1.0f));

        painter->setPen(rectanglePen(ZNAGraphicsObject::displayDigitColor, 0.5f));
        painter->setBrush(ZNAGraphicsObject::displayDigitColor);

        painter->translate(digitBounds.center());
        painter->shear(-0.1, 0.0);
        painter->translate(-digitBounds.center());

        painter->translate(digitBounds.topLeft());
        for (int i = 0; i < 7; ++i)
        {
            const auto segmentPolygon = this->calculateSegmentPolygon(digitBounds.size(), 3.0f, i);
            painter->drawPolygon(segmentPolygon);
        }

        /*const auto segmentPolygon = this->calculateSegmentPolygon(location.size(), 2.0f, 1);
        painter->drawPolygon(segmentPolygon);*/

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

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();

        this->drawDigit(painter, QRectF{boundingRect.left() + 10, boundingRect.top() + 10, 38, 60}, '8');
    }
}
