#include "trackindicatorfeature.hxx"

namespace Openstw::SpDr
{
    TrackIndicatorFeature::TrackIndicatorFeature(TrackIndicatorGeometry geometry, Interlocking::TrackSection* section,
                                                 QObject* parent)
        : FeatureLayer(FeatureType::Track, parent), m_geometry{geometry}, m_trackSection{section}
    {
        // Create aperture for the occupancy indicator.
        auto* occupancyElement = new Aperture("center-occupancy", this);
        this->addAperture(occupancyElement);
        this->m_occupancyLamp = occupancyElement;

        // Listen to occupancy and lock state changes emitted by the track
        // section object we are associated with.
        // clang-format off
        connect(this->m_trackSection, &Interlocking::TrackSection::lockedChanged,
                this, &TrackIndicatorFeature::onLockedChanged);

        connect(this->m_trackSection, &Interlocking::TrackSection::occupancyChanged,
                this, &TrackIndicatorFeature::onOccupancyChanged);
        // clang-format on

        // Perform initial synchronization
        this->updateOccupancyAperture();
    }

    TrackIndicatorGeometry TrackIndicatorFeature::geometry() const
    {
        return this->m_geometry;
    }

    Aperture* TrackIndicatorFeature::occupancyIndicator() const
    {
        return this->m_occupancyLamp;
    }

    void TrackIndicatorFeature::updateOccupancyAperture()
    {
        // Retrieve lock and occupancy state
        const auto isLocked = this->m_trackSection->locked();
        const auto isOccupied = this->m_trackSection->occupied();

        // Default to off
        auto newState = ApertureState::off();

        // If the track section is occupied, always show red,
        // even if the track section is not locked!
        // Because a train might mistakenly have entered a track
        // that is not part of its route (or we have a Rotausleuchtung)
        if (isOccupied)
        {
            newState = ApertureState::solid(ApertureColor::Red);
        }
        else if (isLocked) // Otherwise, a locked state is displayed as a warm white color.
        {
            newState = ApertureState::solid(ApertureColor::White);
        }

        // Apply aperture state. This will signal the UI to redraw itself!
        this->m_occupancyLamp->updateState(newState);
    }

    void TrackIndicatorFeature::onLockedChanged(bool isLocked)
    {
        this->updateOccupancyAperture();
    }

    void TrackIndicatorFeature::onOccupancyChanged(bool isOccupied)
    {
        this->updateOccupancyAperture();
    }
}
