#pragma once

#include "displayelement.hxx"
#include "featurelayer.hxx"
#include "featuretype.hxx"
#include <optional>

namespace Openstw::SpDr
{
    // Feature layer representing a horizontal track, which
    // can have an occupancy indicator.
    class TrackHorizontalFeature : public FeatureLayer
    {
        Q_OBJECT

    public:
        TrackHorizontalFeature(bool withOccupancyIndicator, QObject* parent = nullptr);

        virtual ~TrackHorizontalFeature() override = default;

    protected:
        // Whether this track has a occupancy indicator.
        bool m_hasOccupancy{false};

        // The display element showing the occupancy state
        std::optional<DisplayElement*> m_occupancyLamp{};
    };
}
