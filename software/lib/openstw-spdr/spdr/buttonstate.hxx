#pragma once
#include <QString>
#include <QStringView>

namespace Openstw::SpDr
{
    // The different states a button on a SpDr tile can be in.
    enum class ButtonState
    {
        Released = 0,
        Pressed = 1
    };
}
