#include "signalpainter.hxx"

namespace Rendering
{
    SignalPainter::SignalPainter(BlinkGenerator* blink, QObject* parent)
        : FeaturePainter{blink, parent}
    {
    }

    void SignalPainter::drawSignalLamp(QPainter* painter, const QRectF& location, const Aperture* aperture,
                                       const qreal onDiameter, const qreal offDiameter) const
    {
        painter->save();

        // First, determine the colour to use.
        const auto lampColor = this->resolveColor(aperture);

        // Also determine which diameter to use, based on whether the aperture is on
        // or not.
        const auto diameter = this->apertureIsOn(aperture) ? onDiameter : offDiameter;

        // Finally, draw the signal lamp.
        QRectF lampRect{centerWithin(diameter, location.width(), location.left()),
                        centerWithin(diameter, location.height(), location.top()), diameter, diameter};

        painter->setBrush(lampColor);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(lampRect);

        painter->restore();
    }
}
