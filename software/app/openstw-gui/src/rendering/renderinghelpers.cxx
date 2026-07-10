#include "renderinghelpers.hxx"
#include <QPen>

namespace Rendering
{
    QRectF adjustRectForBorder(const QRectF& rect, const qreal borderThickness)
    {
        return QRectF{rect.left() + borderThickness / 2.0f, rect.top() + borderThickness / 2.0f,
                      rect.width() - borderThickness, rect.height() - borderThickness};
    }

    QPen rectanglePen(const QColor color, const qreal borderThickness)
    {
        return QPen{QBrush{color}, borderThickness, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin};
    }

    qreal centerWithin(qreal objectLength, qreal containerLength, qreal containerPosition)
    {
        return containerPosition + ((containerLength / 2.0f) - (objectLength / 2.0f));
    }

    void drawTextBox(QPainter* painter, const QRectF& rect, const QString& text, QColor backgroundColor,
                     QColor borderColor, qreal borderThickness, QColor textColor, std::optional<qreal> rotation)
    {
        painter->save();

        QRectF adjustedBackgroundRect = rect;
        QPen borderPen = Qt::NoPen;
        QBrush backgroundBrush{backgroundColor};

        if (borderThickness > 0.0)
        {
            adjustedBackgroundRect = adjustRectForBorder(rect, borderThickness);
            borderPen = rectanglePen(borderColor, borderThickness);
        }

        // Draw text box background
        painter->setPen(borderPen);
        painter->setBrush(backgroundBrush);
        painter->drawRect(adjustedBackgroundRect);

        // Draw actual label text
        const auto textRect = rect.marginsRemoved(QMarginsF{3.0f, 1.0f, 3.0f, 1.0f});
        adjustFontSizeToFit(painter, textRect, Qt::TextSingleLine, text);

        if (rotation.has_value())
        {
            painter->translate(textRect.center());
            painter->rotate(rotation.value());
            painter->translate(-textRect.center());
        }

        painter->setPen(QPen{textColor});
        painter->setBrush(backgroundBrush);
        painter->drawText(textRect, Qt::TextSingleLine | Qt::AlignCenter, text);

        painter->restore();
    }

    void adjustFontSizeToFit(QPainter* painter, const QRectF& rect, const int textFlags, const QString& text)
    {
        const int actualFlags = textFlags | Qt::TextDontClip;
        const auto textRect = painter->fontMetrics().boundingRect(rect.toRect(), actualFlags, text);

        float xFactor = rect.width() / textRect.width();
        float yFactor = rect.height() / textRect.height();
        float factor = xFactor < yFactor ? xFactor : yFactor;
        QFont f = painter->font();
        f.setPointSizeF(f.pointSizeF() * factor);
        painter->setFont(f);
    }

    void makeCurrentFontBold(QPainter* painter)
    {
        QFont currentFont = painter->font();
        currentFont.setBold(true);
        painter->setFont(currentFont);
    }
}
