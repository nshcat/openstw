#pragma once

#include "tilecomponentgraphicsobject.hxx"
#include <QObject>
#include <array>

class TileGraphicsObject;

namespace Rendering
{
    class ZNAGraphicsObject : public Rendering::TileComponentGraphicsObject
    {
        Q_OBJECT

        enum class SegmentOrientation
        {
            Horizontal = 0,
            Vertical = 1
        };

    public:
        constexpr static qreal compactDisplayHeight = 32.0f;
        constexpr static qreal largeDisplayHeight = 50.0f;
        constexpr static qreal compactDisplaySidePadding = 4.0f;
        constexpr static qreal largeDisplaySidePadding = 40.0f;
        constexpr static QColor displayBackgroundColor{0x25, 0x25, 0x25};
        constexpr static QColor displayBorderColor{0x3f, 0x3f, 0x3f};
        constexpr static QColor displayDigitColor{0xf9, 0x6f, 0x71};

        constexpr static qreal compactDisplayDigitPadding = 2.0f;
        constexpr static qreal compactDisplayLabelWidth = 120.0f;
        constexpr static qreal compactDisplayLabelHeight = 26.5f;
        constexpr static qreal compactDisplayLabelBottomPadding = 2.0f;

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
        ZNAGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawCompactDisplay(QPainter* painter, const QString& zugnummer, const QString& label) const;

        void drawDigit(QPainter* painter, const QRectF& location, const qreal segmentThickness, const int digit) const;

        QPolygonF calculateSegmentPolygon(const QSizeF digitSize, const qreal segmentThickness,
                                          const int segmentNr) const;

        QPolygonF calculateHorizontalSegmentPolygon(const QSizeF digitSize, const qreal segmentThickness) const;
        QPolygonF calculateVerticalSegmentPolygon(const QSizeF digitSize, const qreal segmentThickness) const;

        void drawCompactLabel(QPainter* painter, const QString& label) const;
    };
}
