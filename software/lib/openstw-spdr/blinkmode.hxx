#pragma once

namespace Openstw::SpDr
{
    // Enumeration describing the different ways a display element
    // can blink.
    enum class BlinkMode
    {
        // Element is not blinking, but illuminated solidly
        Solid = 0,

        // Element is blinking
        Blinking = 1
    };
}