#pragma once

#include "blinkmode.hxx"
#include "elementcolor.hxx"

namespace Openstw::SpDr
{
    // Class describing the current state of a display element.
    class ElementState
    {
    public:
        ElementState(ElementColor color, BlinkMode blinking);

    public:
        // Creates element state that describes an element that is turned off.
        static ElementState off();

        // Creates element state that describes a solid colour.
        static ElementState solid(ElementColor color);

        // Creates element state that describes a blinking colour.
        static ElementState blinking(ElementColor color);

    public:
        // Colour of the element
        ElementColor color() const;

        // Blinking mode of the element
        BlinkMode blinkMode() const;

        // Whether the element is currently off
        bool isOff() const;

    public:
        bool operator==(const ElementState& rhs) const = default;
        bool operator!=(const ElementState& rhs) const = default;

    protected:
        ElementColor m_color{ElementColor::Off};
        BlinkMode m_blinkMode{BlinkMode::Solid};
    };
}
