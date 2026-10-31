#pragma once

#include <QObject>

namespace Openstw::Interlocking
{
    namespace ElementTypeNS
    {
        Q_NAMESPACE

        // Enumeration describing all the different types of elements
        // within the interlocking system.
        enum class ElementType
        {
            // A track section is a section within the track network that is
            // regarded as one indivisible unit in regards to occupancy detection.
            // It links to axle counter nodes within the topology graph and labels
            // them with either "entry" or "exit", reacting to their traversal
            // by incrementing or decrementing an internal axle counter value, which
            // in turn determines the occupancy status.
            TrackSection = 0,

            // A mast containing a number of Signalschirme, among them things
            // like Hautpsignale, Vorsignale, Zusatzsignale, Sperrsignale etc.
            // Every kind of signal (except virtual signals) is handled by
            // this element type.
            // It has a reference to a Signal topography node.
            SignalMast = 1,

            // A virtual signal that acts as a route destination for setting
            // Fahrstrassen into a Selbst- or Zentralblock.
            RouteDestination = 2,
        };

        Q_ENUM_NS(ElementType)
    }

    using namespace ElementTypeNS;
}