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
            // All signal lamps are off, this is usually the case
            // if the Vorsignal is part of a Signalmast that also
            // contains a Hauptsignal, and that Signal is showing halt.
            Off = 0,

            HaltErwarten = 1,

            FahrtErwarten = 2,

            LangsamFahrtErwarten = 3
        };

        Q_ENUM_NS(VorSignalBild)
    }

    using namespace VorSignalBildNS;
}