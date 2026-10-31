#pragma once

namespace Openstw
{
    namespace Topology
    {
        // Enumeration listing the different general categories
        // of elements within the topology network.
        enum class ElementCategory
        {
            // Element is a node in the topology graph
            Node,

            // Element is an edge in the topology graph
            Edge,
        };
    }
}