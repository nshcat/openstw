#include "trackhorizontalfeature.hxx"

namespace Openstw::SpDr
{
    TrackHorizontalFeature::TrackHorizontalFeature(bool withOccupancyIndicator, QObject* parent)
        : FeatureLayer(FeatureType::TrackHorizontal, parent), m_hasOccupancy{withOccupancyIndicator}
    {
        // If this track contains an occupancy indicator, fetch the
        // corresponding track segment and setup display element
        // that shows the track segments occupancy state.
        if (withOccupancyIndicator)
        {
            // XXX We need the track segment from the interlocking layer
            // whose state the occupancy indicator on this feature layer
            // displays.
            // The lamp is always the center one.
            auto* occupancyElement = new Aperture("center-occupancy", this);

            this->addAperture(occupancyElement);

            this->m_occupancyLamp = {occupancyElement};

            // XXX connect to track segment object signals to change our
            // display elements state.
        }
    }
}
