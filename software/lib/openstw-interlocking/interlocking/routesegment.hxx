#pragma once

#include "element.hxx"
#include "elements/signal.hxx"
#include "elements/switch.hxx"
#include "elements/tracksection.hxx"
#include "routesegmenttype.hxx"
#include "switchposition.hxx"
#include <optional>

namespace Openstw::Interlocking
{
    // A single segment within a route, containing a reference
    // to an element within the interlocking network.
    // For switches, this also contains a required switch position
    // for the route to lock in.
    class RouteSegment
    {
    public:
        // Create route based on given element.
        RouteSegment(Element* element, RouteSegmentType category);

        // Create route segment containing a switch.
        RouteSegment(Switch* switchElement);

        // Create route segment containing a signal.
        RouteSegment(Signal* signalElement);

        // Create route segment containing a track section.
        RouteSegment(TrackSection* sectionElement);

    public:
        // The category of element this route segment represents.
        RouteSegmentType type() const;

        // The element this route segment represents.
        Element* element() const;

        // Whether this route segment represents a switch.
        bool isSwitch() const;

        // Retrieve the switch this route segment is representing.
        Switch* asSwitch() const;

        // If this route segment is a switch, its target position.
        SwitchPosition targetPosition() const;

        // Whether this route segment represents a signal.
        bool isSignal() const;

        // Retrieve the signal this route segment is representing.
        Signal* asSignal() const;

        // Whether this route segment represents a track section.
        bool isSection() const;

        // Retrieve the track section this route segment is representing.
        TrackSection* asSection() const;

    protected:
        // The category of element this route segment represents.
        RouteSegmentType m_type{};

        // The element this route segment represents.
        Element* m_element;

        // If this route segment represents a switch, its desired
        // position.
        std::optional<SwitchPosition> m_switchPosition{};
    };
}
