#pragma once

#include <QtCore>

namespace Rendering
{
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
}
