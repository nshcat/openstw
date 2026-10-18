#pragma once

#include <cstdint>

namespace Openstw
{
    namespace Topology
    {
        // Descriptors for the various ports a topology graph node can have.
        enum class Port : std::uint32_t
        {
            // For nodes with a single port, such as bumpers and boundary
            // nodes
            Center,

            // For nodes that have two ports,
            // such as signals, connectors and axle counters
            Front,
            Back,

            // For switches
            Wurzel,
            Gerade,
            Abzweig
        };
    }
}