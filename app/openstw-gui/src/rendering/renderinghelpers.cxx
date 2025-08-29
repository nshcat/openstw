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
}
