#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace RouteSegmentTypeNS
    {
        Q_NAMESPACE

        // XXXTODO Just define a ElementCategory enumeration,
        // and let Element store it, instead of having this super
        // specific enum that only the Route class uses..

        // The different kinds of element categories
        // a route segment can represent.
        enum class RouteSegmentType
        {
            TrackSection = 0,

            Signal = 1,

            Switch = 2
        };

        Q_ENUM_NS(RouteSegmentType)
    }

    using namespace RouteSegmentTypeNS;
}