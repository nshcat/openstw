#include "featuretype.hxx"

namespace Openstw::SpDr
{
    std::optional<FeatureType> parseFeatureType(QStringView featureTypeName)
    {
        if (featureTypeName == "Unknown")
        {
            return FeatureType::Unknown;
        }
        else if (featureTypeName == "TrackHorizontal")
        {
            return FeatureType::TrackHorizontal;
        }

        return std::nullopt;
    }

    QString featureTypeToString(FeatureType featureType)
    {
        switch (featureType)
        {
        case FeatureType::Unknown:
            return "Unknown";
        case FeatureType::TrackHorizontal:
            return "TrackHorizontal";
        default:
            return "<unknown>";
        }
    }
}