#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace RequestStateNS
    {
        Q_NAMESPACE

        // The different states a request can be in
        enum class RequestState
        {
            // Request hasnt been processed yet by the interlocking
            // system.
            Pending = 0,

            // The interlocking system has denied the request.
            // The exact reason for the denial can be queried on the request.
            Denied = 1,

            // Request was compeleted by the interlocking system.
            Completed = 2,
        };

        Q_ENUM_NS(RequestState)
    }

    using namespace RequestStateNS;
}