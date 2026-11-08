#pragma once
#include <QString>
#include <optional>

namespace Openstw::SpDr
{
    namespace FeatureTypeNS
    {
        Q_NAMESPACE

        // Enumeration describing the different types of feature layers
        // a SpDr can have.
        enum class FeatureType
        {
            // Track with occupancy indicator.
            TrackIndicator = 0
        };

        Q_ENUM_NS(FeatureType)
    }

    using namespace FeatureTypeNS;
}