#pragma once

#include <QString>
#include <QStringView>
#include <optional>

namespace Openstw::Interlocking
{
    // Facing of an axle counter in relation to one specific
    // track section.
    enum class AxleCounterFacing
    {
        // Traversing the axle counter from back to front
        // implies entering the track section.
        Inward,

        // Traversing the axle counter from front to back
        // implies entering the track section.
        Outward
    };

    // Parse facing enum value from string representation.
    std::optional<AxleCounterFacing> parseAxleCounterFacing(QStringView facingName);

    // Convert facing enum value to string representation.
    QString axleCounterFacingToString(AxleCounterFacing facing);
}
