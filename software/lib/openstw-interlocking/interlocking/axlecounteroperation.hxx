#pragma once

namespace Openstw::Interlocking
{
    // Operations that can be performed on an axle counter value.
    enum class AxleCounterOperation
    {
        // Count up the axle counter value, indicating
        // that an axle has entered the associated track section.
        CountUp = 0,

        // Count down the axle counter value, indicating
        // that an axle has left the associated track section.
        CountDown = 1
    };
}