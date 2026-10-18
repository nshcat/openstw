#pragma once

#include <cstdint>

namespace Openstw
{
    namespace Topology
    {
        // Enumeration describing all the possible edge types within
        // the topology graph.
        enum class EdgeType : std::uint32_t
        {
            // Track element. Has a maximum speed and a length associated with it.
            Track
        };
    }
}
