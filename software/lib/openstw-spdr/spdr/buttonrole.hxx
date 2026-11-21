#pragma once

#include <QObject>

namespace Openstw::SpDr
{
    namespace ButtonRoleNS
    {
        Q_NAMESPACE

        // The different roles a SpDr tile button can have.
        enum class ButtonRole
        {
            // Button is associated with a signal.
            SignalTaste = 0,

            // Button is associated with a shunting signal.
            RangierSignalTaste = 1,

            // Button is associated with a switch.
            WeichenTaste = 2,
        };

        Q_ENUM_NS(ButtonRole)
    }

    using namespace ButtonRoleNS;
}