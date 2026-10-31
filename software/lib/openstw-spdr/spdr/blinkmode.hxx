#pragma once

namespace Openstw::SpDr
{
    // Enumeration describing the different ways an aperture
    // can blink.
    enum class BlinkMode
    {
        // Element is not blinking, but illuminated solidly
        Solid = 0,

        // Element is blinking
        Blinking = 1
    };
}