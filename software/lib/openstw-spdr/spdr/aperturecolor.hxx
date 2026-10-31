#pragma once

namespace Openstw::SpDr
{
    // Enumeration describing the different colours a
    // aperture can light up in.
    enum class ApertureColor
    {
        // Aperture is off.
        Off = 0,

        Red = 1,
        Green = 2,
        Yellow = 3,

        // A warm, incandescant white (real SpDr tiles use incandesant lamps)
        White = 4,

        Blue = 5
    };
}
