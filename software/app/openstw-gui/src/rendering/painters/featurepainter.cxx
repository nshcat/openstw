#include "featurepainter.hxx"

namespace Rendering
{
    FeaturePainter::FeaturePainter(BlinkGenerator* blink, QObject* parent)
        : QObject{parent}, m_blink{blink}
    {
    }

    BlinkGenerator* FeaturePainter::blinkGenerator() const
    {
        return this->m_blink;
    }

    QColor FeaturePainter::resolveColor(const Openstw::SpDr::Aperture* aperture) const
    {
        // Retrieve current aperture state.
        const auto& state = aperture->state();

        // If the aperture is blinking, we need to do more work.
        if (state.isBlinking())
        {
            // If the blink generator is currently indicating that blinking
            // lamps should be on, translate the apertures color.
            if (this->blinkGenerator()->lampsOn())
            {
                return convertApertureColor(state.color());
            }
            else
            {
                // Otherwise, return the Qt color for 'Off' aperture state.
                return convertApertureColor(Openstw::SpDr::ApertureColor::Off);
            }
        }
        else // Otherwise, just translate the aperture colour directly.
        {
            return convertApertureColor(state.color());
        }
    }

    bool FeaturePainter::apertureIsOn(const Aperture* aperture) const
    {
        // Retrieve the apertures current state.
        const auto& currentState = aperture->state();

        // We need to do special handling if the aperture is blinking.
        if (currentState.blinkMode() == Openstw::SpDr::BlinkMode::Blinking)
        {
            return this->blinkGenerator()->lampsOn();
        }

        // Otherwise, check whether the aperture is off.
        return !currentState.isOff();
    }
}
