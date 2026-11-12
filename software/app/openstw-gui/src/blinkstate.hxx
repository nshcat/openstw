#pragma once

#include <QObject>

namespace BlinkStateNS
{
    Q_NAMESPACE

    // The two states a blink impulse generator can be in.
    enum class BlinkState
    {
        LampOff = 0,
        LampOn = 1
    };

    Q_ENUM_NS(BlinkState)
}

using namespace BlinkStateNS;