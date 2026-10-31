#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace RouteStateNS
    {
        Q_NAMESPACE

        // The different states a route can be in.
        enum class RouteState
        {
            // Route is found and defined, but not activated yet.
            Defined = 0,

            // Route is committed, and the elements it is comprised of
            // are currently being locked.
            Locking = 1,

            // Route is fully locked, and train can drive.
            Locked = 2,

            // Route is released and no longer locked, either because
            // a passing train has released it, or it was forcefully released.
            Released = 3
        };

        Q_ENUM_NS(RouteState)
    }

    using namespace RouteStateNS;
}