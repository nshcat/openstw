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
        /* clang-format on */

    public:
        ZNAGraphicsObject(TileGraphicsObject* parent);

    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        virtual void positionSelf() override;

    protected:
        void drawDigit(QPainter* painter, const QRectF& location, const char digit) const;

        QPolygonF calculateSegmentPolygon(const QSizeF digitSize, const qreal segmentThickness,
                                          const int segmentNr) const;

        QPolygonF calculateHorizontalSegmentPolygon(const QSizeF digitSize, const qreal segmentThickness) const;
        QPolygonF calculateVerticalSegmentPolygon(const QSizeF digitSize, const qreal segmentThickness) const;
    };
}
