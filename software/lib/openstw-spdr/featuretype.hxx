#pragma once
#include <QString>
#include <optional>

namespace Openstw::SpDr
{
    // Enumeration describing the different types of feature layers
    // a SpDr can have.
    enum class FeatureType
    {
        Unknown = 0,

        // Simple, horizontal track. Might possibly contain a
        // occupancy indicator.
        TrackHorizontal = 1
    };

    // Parse feature type enum value from string representation.
    std::optional<FeatureType> parseFeatureType(QStringView featureTypeName);

    // Convert feature type enum value to string representation.
    QString featureTypeToString(FeatureType featureType);
}