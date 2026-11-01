#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace RequestDenialReasonNS
    {
        Q_NAMESPACE

        // The different reasons as to why a request was denied by
        // the interlocking system.
        enum class RequestDenialReason
        {
            // Unknown reason
            Unknown = 0,

            // Route was invalid
            InvalidRoute = 1,
        };

        Q_ENUM_NS(RequestDenialReason)
    }

    using namespace RequestDenialReasonNS;
}