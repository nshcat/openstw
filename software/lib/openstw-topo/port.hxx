#pragma once

#include <QStringView>
#include <cstdint>
#include <iostream>
#include <optional>

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

        // Parse port enum value from string representation.
        std::optional<Port> parsePort(QStringView portName);

        // Convert port enum value to string representation.
        QString portToString(Port port);

        // Stream output operator for ports
        std::ostream& operator<<(std::ostream& os, const Port& port);

        // Stream input operator for ports
        std::istream& operator>>(std::istream& is, Port& port);
    }
}