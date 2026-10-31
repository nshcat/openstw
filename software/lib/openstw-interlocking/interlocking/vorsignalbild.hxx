#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace VorSignalBildNS
    {
        Q_NAMESPACE

        // The different aspects a Vorsignal can show.
        enum class VorSignalBild
        {
            HaltErwarten = 0,
            FahrtErwarten = 1,
            LangsamFahrtErwarten = 2
        };

        Q_ENUM_NS(VorSignalBild)
    }

    using namespace VorSignalBildNS;
}