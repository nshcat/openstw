#pragma once

#include <QObject>
#include <QPainter>
#include <QRectF>
#include <QSizeF>

#include <spdr/aperture.hxx>

#include "blinkgenerator.hxx"
#include "renderinghelpers.hxx"

namespace Rendering
{
    using Aperture = Openstw::SpDr::Aperture;

    // Base class for painter classes that paint specific elements
    // on tile features.
    class FeaturePainter : public QObject
    {
        Q_OBJECT

    public:
        // Base class constructor for feature painters.
        explicit FeaturePainter(BlinkGenerator* blink, QObject* parent = nullptr);
        virtual ~FeaturePainter() = default;

    public:
        // Reference to blink generator that is used to render
        // blinking apertures.
        BlinkGenerator* blinkGenerator() const;

    public:
        // Measure out the feature to be painted, returning its bounding box
        // that can be used to correctly lay the feature out.
        // The returned size can be positioned by the caller as they see fit,
        // before passing it as a rectangle to `FeaturePainter::paint` for rendering.
        virtual QSizeF measure() = 0;

        // Actually draw the feature within the confines of given rectangle.
        virtual void paint(QPainter* painter, const QRectF& where) = 0;

    protected: // Helper methods that can be used for painting.
        // Determine the current Qt color to use for rendering given aperture.
        // This also takes into account the current blink generator state.
        QColor resolveColor(const Aperture* aperture) const;

        // Determine whether the given aperture is currently on. This correctly
        // incorporates the global blink timer when it comes to blinking apertures.
        bool apertureIsOn(const Aperture* aperture) const;

    protected:
        // Blink impulse generator needed to render blinking
        // apertures.
        BlinkGenerator* m_blink;
    };
}
