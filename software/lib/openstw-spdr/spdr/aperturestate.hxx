#pragma once

#include "blinkmode.hxx"
#include "aperturecolor.hxx"

namespace Openstw::SpDr
{
    // Class describing the current state of an aperture.
    class ApertureState
    {
    public:
        ApertureState(ApertureColor color, BlinkMode blinking);

    public:
        // Creates aperture state that describes an aperture that is turned off.
        static ApertureState off();

        // Creates aperture state that describes a solid colour.
        static ApertureState solid(ApertureColor color);

        // Creates aperture state that describes a blinking colour.
        static ApertureState blinking(ApertureColor color);

    public:
        // Colour of the aperture
        ApertureColor color() const;

        // Blinking mode of the aperture
        BlinkMode blinkMode() const;

        // Whether the aperture is currently off
        bool isOff() const;

    public:
        bool operator==(const ApertureState& rhs) const = default;
        bool operator!=(const ApertureState& rhs) const = default;

    protected:
        ApertureColor m_color{ApertureColor::Off};
        BlinkMode m_blinkMode{BlinkMode::Solid};
    };
}
