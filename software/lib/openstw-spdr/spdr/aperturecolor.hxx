#pragma once

#include <QString>

namespace Openstw::SpDr
{
    namespace ApertureColorNS
    {
        Q_NAMESPACE

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

        Q_ENUM_NS(ApertureColor)
    }

    using namespace ApertureColorNS;
}
