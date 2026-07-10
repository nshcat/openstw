#include "sevensegmentrenderer.hxx"
#include "renderinghelpers.hxx"

namespace Rendering
{
    SevenSegmentRenderer::SevenSegmentRenderer()
    {
    }

    void SevenSegmentRenderer::setup(const QSizeF& digitAreaSize, const qreal segmentThickness)
    {
        this->m_digitAreaSize = digitAreaSize;
        this->m_digitSize = QSizeF{digitAreaSize.width() - 8, digitAreaSize.height()};
        this->m_segmentThickness = segmentThickness;

        this->recalculateHorizontalSegmentPolygon();
        this->recalculateVerticalSegmentPolygon();
        this->recalculateSegmentOffsets();

        this->m_isSetup = true;
    }

    void SevenSegmentRenderer::drawDigit(QPainter* painter, const QRectF& location, QColor digitColor, int digit) const
    {
        if (!this->m_isSetup)
            throw std::runtime_error("Renderer not setup");

        painter->save();

        if (digit < 0 || digit > 9)
            throw std::runtime_error("Digit out of range");

        const QRectF digitBounds = {centerWithin(this->m_digitSize.width(), location.width(), location.left()),
                                    centerWithin(this->m_digitSize.height(), location.height(), location.top()),
                                    this->m_digitSize.width(), this->m_digitSize.height()};

        painter->setPen(rectanglePen(digitColor, 0.5f));
        painter->setBrush(digitColor);

        painter->translate(digitBounds.center());
        painter->shear(-0.1, 0.0);
        painter->translate(-digitBounds.center());
        painter->translate(digitBounds.topLeft());

        const auto segmentStates = SevenSegmentRenderer::digitSegments[digit];
        for (int i = 0; i < 7; ++i)
        {
            if ((segmentStates >> i) & 1)
            {
                const auto segmentPolygon = this->calculateSegmentPolygon(i);
                painter->drawPolygon(segmentPolygon);
            }
        }

        painter->restore();
    }

    const QSizeF& SevenSegmentRenderer::digitAreaSize() const
    {
        return this->m_digitAreaSize;
    }

    qreal SevenSegmentRenderer::segmentThickness() const
    {
        return this->m_segmentThickness;
    }

    QPolygonF SevenSegmentRenderer::calculateSegmentPolygon(const int segmentNr) const
    {
        if (segmentNr < 0 || segmentNr >= 7)
            throw std::runtime_error("Invalid segment number");

        const auto segmentOrientation = SevenSegmentRenderer::segmentOrientations[segmentNr];
        QPolygonF baseSegmentPolygon{};
        if (segmentOrientation == SegmentOrientation::Vertical)
            baseSegmentPolygon = this->m_vertSegmentPolygon;
        else
            baseSegmentPolygon = this->m_horzSegmentPolygon;

        const QPolygonF segmentPolygon = baseSegmentPolygon.translated(this->m_segmentOffsets[segmentNr]);

        return segmentPolygon;
    }

    void SevenSegmentRenderer::recalculateHorizontalSegmentPolygon()
    {
        const qreal halfThickness = this->m_segmentThickness / 2.0f;

        QPolygonF segment{};

        segment << QPointF{halfThickness, halfThickness} << QPointF{2 * halfThickness, 0.0f}
                << QPointF{this->m_digitSize.width() - 2 * halfThickness, 0.0}
                << QPointF{this->m_digitSize.width() - halfThickness, halfThickness}
                << QPointF{this->m_digitSize.width() - 2 * halfThickness, this->m_segmentThickness}
                << QPointF{2 * halfThickness, this->m_segmentThickness} << QPointF{halfThickness, halfThickness};

        this->m_horzSegmentPolygon = segment;
    }

    void SevenSegmentRenderer::recalculateVerticalSegmentPolygon()
    {
        const qreal halfThickness = this->m_segmentThickness / 2.0f;

        QPolygonF segment{};

        segment << QPointF{halfThickness, halfThickness} << QPointF{this->m_segmentThickness, 2 * halfThickness}
                << QPointF{this->m_segmentThickness, this->m_digitSize.height() / 2.0f - 2 * halfThickness}
                << QPointF{halfThickness, this->m_digitSize.height() / 2.0f - halfThickness}
                << QPointF{0.0, this->m_digitSize.height() / 2.0f - 2 * halfThickness}
                << QPointF{0.0, 2 * halfThickness} << QPointF{halfThickness, halfThickness};

        this->m_vertSegmentPolygon = segment;
    }

    void SevenSegmentRenderer::recalculateSegmentOffsets()
    {
        const qreal halfHeight = this->m_digitSize.height() / 2.0f;
        const std::array<QPointF, 7> segmentOffsets{
            QPointF{0.0f, 0.0f},
            QPointF{this->m_digitSize.width() - this->m_segmentThickness, 0.0f},
            QPointF{this->m_digitSize.width() - this->m_segmentThickness, halfHeight},
            QPointF{0.0f, this->m_digitSize.height() - this->m_segmentThickness},
            QPointF{0.0f, halfHeight},
            QPointF{0.0f, 0.0f},
            QPointF{0.0f, halfHeight - this->m_segmentThickness / 2.0f}};

        this->m_segmentOffsets = segmentOffsets;
    }
}
