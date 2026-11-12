#pragma once

#include <optional>

#include <QPainter>
#include <QtCore>

namespace Rendering
{
    enum class HorizontalDirection
    {
        Left,
        Right
    };

    enum class VerticalDirection
    {
        Top,
        Bottom
    };

    /**
     * @brief Adjusts given rect in such a way that when drawn with a pen of given thickness,
     * everything rendered (including the entirety of the border) will lay entirely inside the rectangle.
     *
     * With QPainters drawRect method, this would otherwise not be the case, since the
     * border line is centered on the 'ideal' rectangle.
     */
    QRectF adjustRectForBorder(const QRectF& rect, const qreal borderThickness);

    /**
     * @brief Creates a pen to be used with drawing perfectly rectangular rectangles, without any
     * corner rounding.
     */
    QPen rectanglePen(const QColor color, const qreal borderThickness);

    /**
     * @brief Centers an object within a containing object in one dimension. For 2D objects,
     * use this function on both the x and y coordinate with appropiate values.
     */
    qreal centerWithin(qreal objectLength, qreal containerLength, qreal containerPosition);

    /**
     * @brief Draws a text box that contains given text. The font size is chosen in such a way that the
     * text fills the given rectangle as best as possible.
     */
    void drawTextBox(QPainter* painter, const QRectF& rect, const QString& text, QColor backgroundColor,
                     QColor borderColor, qreal borderThickness, QColor textColor,
                     std::optional<qreal> rotation = std::nullopt);

    /**
     * @brief Adjusts the size of the currently set font in such a way that the given text fits well into the given
     * rectangle.
     */
    void adjustFontSizeToFit(QPainter* painter, const QRectF& rect, const int textFlags, const QString& text);

    /**
     * @brief Adjusts the currently active font to be bold
     */
    void makeCurrentFontBold(QPainter* painter);
}
