#pragma once

namespace Openstw::Interlocking
{
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
    };
}