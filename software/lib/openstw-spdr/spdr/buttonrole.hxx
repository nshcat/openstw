#pragma once

namespace Openstw::SpDr
{
    // The different roles a SpDr tile button can have.
    enum class ButtonRole
    {
        // Button is associated with a signal.
        SignalTaste = 0,

        // Button is associated with a shunting signal.
        RangierSignalTaste = 1,
    };
}