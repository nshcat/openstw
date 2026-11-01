#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace SwitchPositionNS
    {
        Q_NAMESPACE

        // The different positions a switch can be in.
        enum class SwitchPosition
        {
            // Switch is in straight position, connecting
            // Wurzel and Gerade.
            Straight = 0,

            // Switch is in diverging position, connecting
            // Wurzel and Abzweig.
            Diverging = 1,
        };

        Q_ENUM_NS(SwitchPosition)
    }

    using namespace SwitchPositionNS;
}