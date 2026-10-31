#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace HauptSignalBildNS
    {
        Q_NAMESPACE

        // The different aspects a Hauptsignal can show.
        enum class HauptSignalBild
        {
            Halt = 0,
            Fahrt = 1,
            LangsamFahrt = 2
        };

        Q_ENUM_NS(HauptSignalBild)
    }

    using namespace HauptSignalBildNS;
}