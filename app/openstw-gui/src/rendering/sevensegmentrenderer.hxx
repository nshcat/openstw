#pragma once

#include <QPainter>
#include <QtCore>
#include <array>

namespace Rendering
{
    class SevenSegmentRenderer
    {
    protected:
        enum class SegmentOrientation
        {
            Horizontal = 0,
            Vertical = 1
        };

        /* clang-format off */
         
        /**
         *    0
         *    -
         * 5| 6 | 1
         *    -
         * 4| 3 | 2
         *    -
         */
        constexpr static std::array<SegmentOrientation, 7> segmentOrientations{
            SegmentOrientation::Horizontal,
            SegmentOrientation::Vertical,
            SegmentOrientation::Vertical,
            SegmentOrientation::Horizontal,
            SegmentOrientation::Vertical,
            SegmentOrientation::Vertical,
            SegmentOrientation::Horizontal
        };

        constexpr static std::array<std::uint8_t, 10> digitSegments{
            0b0111111,  // 0
            0b0000110,  // 1
            0b1011011,  // 2
            0b1001111,  // 3
            0b1100110,  // 4
            0b1101101,  // 5
            0b1111101,  // 6
            0b0000111,  // 7
            0b1111111,  // 8
        };
        /* clang-format on */

    public:
        SevenSegmentRenderer();

    public:
        void setup(const QSizeF& digitAreaSize, const qreal segmentThickness);
        void drawDigit(QPainter* painter, const QRectF& location, QColor digitColor, int digit) const;

    public:
        const QSizeF& digitAreaSize() const;
        qreal segmentThickness() const;

    protected:
        QPolygonF calculateSegmentPolygon(const int segmentNr) const;

        void recalculateHorizontalSegmentPolygon();
        void recalculateVerticalSegmentPolygon();
        void recalculateSegmentOffsets();

    protected:
        bool m_isSetup{false};
        QSizeF m_digitSize{};
        QSizeF m_digitAreaSize{};
        qreal m_segmentThickness{};
        QPolygonF m_horzSegmentPolygon{};
        QPolygonF m_vertSegmentPolygon{};
        std::array<QPointF, 7> m_segmentOffsets{};
    };
}
