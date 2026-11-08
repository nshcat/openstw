#pragma once
#include <QString>
#include <optional>

namespace Openstw::SpDr
{
    namespace TrackIndicatorGeometryNS
    {
        Q_NAMESPACE

        // Enumeration describing the different track geometries a
        // SpDr tile can show that do contain track indicators.
        enum class TrackIndicatorGeometry
        {
            Horizontal = 0
        };

        Q_ENUM_NS(TrackIndicatorGeometry)
    }

    using namespace TrackIndicatorGeometryNS;
}