#pragma once

#include <QObject>

#include "featurepainter.hxx"

namespace Rendering
{
    // Base class for feature painters that draw signals, containing
    // useful helper methods.
    class SignalPainter : public FeaturePainter
    {
        Q_OBJECT

    public:
        explicit SignalPainter(BlinkGenerator* blink, QObject* parent = nullptr);

    protected:
        // Draw a single signal lamp.
        void drawSignalLamp(QPainter* painter, const QRectF& location, const Aperture* aperture, const qreal onDiameter,
                            const qreal offDiameter) const;
    };
}
