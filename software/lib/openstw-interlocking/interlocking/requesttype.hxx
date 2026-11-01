#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace RequestTypeNS
    {
        Q_NAMESPACE

        // The different types of requests that can be sent to the
        // interlocking system.
        enum class RequestType
        {
            // Establish a train route between two signals.
            SetZugStrasse = 0
        };

        Q_ENUM_NS(RequestType)
    }

    using namespace RequestTypeNS;
}