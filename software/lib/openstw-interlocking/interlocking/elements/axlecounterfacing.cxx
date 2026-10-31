#include "axlecounterfacing.hxx"

namespace Openstw::Interlocking
{
    std::optional<AxleCounterFacing> parseAxleCounterFacing(QStringView facingName)
    {
        if (facingName == "Inward")
        {
            return AxleCounterFacing::Inward;
        }
        else if (facingName == "Outward")
        {
            return AxleCounterFacing::Outward;
        }

        return std::nullopt;
    }

    QString axleCounterFacingToString(AxleCounterFacing facing)
    {
        switch (facing)
        {
        case AxleCounterFacing::Inward:
            return "Inward";
        case AxleCounterFacing::Outward:
            return "Outward";
        default:
            return "<unknown>";
        }
    }
}
