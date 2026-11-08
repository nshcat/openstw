#include "trackindicatorfeature.hxx"
#include <core/enums.hxx>
#include <core/exception.hxx>

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

    TrackIndicatorFeature* TrackIndicatorFeature::createFromXml(QDomElement xmlElement,
                                                                Interlocking::System* interlocking, QObject* parent)
    {
        // First, we need to figure out the track geometry.
        const auto geometryStr = xmlElement.attribute("Geometry");
        if (geometryStr.isEmpty())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Track indicator feature element missing geometry attribute",
                                     xmlElement.lineNumber());
        }

        // Try to retrieve enum value for it.
        const auto geometry = Core::fromString<TrackIndicatorGeometry>(geometryStr);
        if (!geometry.has_value())
        {
            throw Core::RuntimeError(
                "SpDr XML: Line {}: Track indicator feature element specifies invalid geometry '{}'",
                xmlElement.lineNumber(), geometryStr);
        }

        // Now, retrieve the track section that fills this feature with data.
        const auto sectionStr = xmlElement.attribute("TrackSection");
        if (sectionStr.isEmpty())
        {
            throw Core::RuntimeError(
                "SpDr XML: Line {}: Track indicator feature element missing track section attribute",
                xmlElement.lineNumber());
        }

        // Try to find the element within the interlocking system
        // that represents the referenced track section
        auto sectionElement = interlocking->element(sectionStr);
        if (!sectionElement.has_value())
        {
            throw Core::RuntimeError(
                "SpDr XML: Line {}: Track indicator feature element references non-existent track section '{}'",
                xmlElement.lineNumber(), sectionStr);
        }

        // Validate that the element is, in fact, a track section
        if (sectionElement.value()->type() != Interlocking::ElementType::TrackSection)
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Track indicator feature element references interlocking "
                                     "element '{}', which is not a track section",
                                     xmlElement.lineNumber(), sectionStr);
        }

        // Cast it.
        auto* section = dynamic_cast<Interlocking::TrackSection*>(sectionElement.value());

        // Validate the cast.
        if (!section)
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Track indicator feature element references interlocking "
                                     "element '{}', which unexpectedly is not a TrackSection object",
                                     xmlElement.lineNumber(), sectionStr);
        }

        // Create feature layer and return it.
        return new TrackIndicatorFeature{geometry.value(), section, parent};
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
