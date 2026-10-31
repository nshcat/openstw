#pragma once

#include <QMetaObject>

namespace Openstw::Interlocking
{
    namespace LockingReasonNS
    {
        Q_NAMESPACE

        // The different reasons as to why an interlocking element
        // might be locked.
        // Not all elements support all reasons!
        enum class LockingReason
        {
            // Element is locked because it is part of an active route.
            // This is used for switches and track sections.
            Route = 0,

            // Element is locked for flank protection.
            // This is used for switches and some signals that are
            // designed to provide flank protection.
            FlankProtection = 1,

            // Element is the destination of a route.
            // This is used for signals.
            RouteDestination = 2,

            // Element is the start of a route.
            // This is used for signals.
            RouteStart = 3,
        };

        Q_ENUM_NS(LockingReason)
    }

    using namespace LockingReasonNS;
}