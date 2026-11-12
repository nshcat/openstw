#pragma once

#include "../aperture.hxx"
#include "../featurelayer.hxx"
#include "../featuretype.hxx"
#include "trackindicatorgeometry.hxx"
#include <QDomElement>
#include <interlocking/elements/tracksection.hxx>
#include <interlocking/system.hxx>
#include <optional>

namespace Openstw::SpDr
{
    // Feature layer representing a track, with an occupancy indicator.
    // It has a reference to a track section within the interlocking system,
    // which it uses to derive the state of its occupancy indicator.
    class TrackIndicatorFeature : public FeatureLayer
    {
        Q_OBJECT

    public:
        TrackIndicatorFeature(TrackIndicatorGeometry geometry, Interlocking::TrackSection* section,
                              QObject* parent = nullptr);

        virtual ~TrackIndicatorFeature() override = default;

    public:
        // Parse track indicator feature from given XML element.
        static TrackIndicatorFeature* createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                                    QObject* parent = nullptr);

    public:
        // The specific track geometry this feature describes.
        TrackIndicatorGeometry geometry() const;

        // The aperture dispalying the occupancy state.
        Aperture* occupancyIndicator() const;

    protected:
        // Update the state of the occupancy indicator aperture,
        // based on the current lock and occupancy state of the
        // track section associated with this feature.
        void updateOccupancyAperture();

    protected slots:
        // Called whenever a locked changed signal is emitted by the
        // associated track section.
        void onLockedChanged(bool isLocked);

        // Called whenever a occupied changed signal is emitted by the
        // associated track section.
        void onOccupancyChanged(bool isOccupied);

    protected:
        // The specific track geometry this feature describes.
        TrackIndicatorGeometry m_geometry;

        // The interlocking track section whose state this track indicator displays.
        Interlocking::TrackSection* m_trackSection;

        // The display element showing the occupancy state
        Aperture* m_occupancyLamp{};
    };
}
