#pragma once

#include <QObject>

namespace Openstw::SpDr
{
    namespace SwitchStyleNS
    {
        Q_NAMESPACE;

        // Enumeration describing the differerent styles
        // a simple switch can be shown on a SpDr tile.
        enum class SwitchStyle
        {
            // Most common style, where the switch branch is at a
            // shallow angle - it leaves the tile exactly at the corner.
            // The switch button is in the center.
            Standard,

            // Variant of the common style where the switch branch is at a very
            // steep angle - it leaves the tile at roughly the center of the upper
            // or lower edge of the tile. The switch button is offset from the middle
            // (second or fourth tile grid column).
            Steep,

            // A switch where both the straight and branching arm are leaving the style
            // at both the upper and lower diagonal corners. The switch button is located
            // in the center.
            SymmetricFork,

            // Special version of the standard switch where the stem is also on the diagonal.
            DiagonalStem
        };

        Q_ENUM_NS(SwitchStyle);
    }

    using namespace SwitchStyleNS;
}