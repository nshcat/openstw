#include <QString>

#include "utility.hxx"

namespace Openstw::Simulation
{
    GridPosition::GridPosition(std::size_t x, std::size_t y) : x{x}, y{y}
    {
    }

    TileElementDirection Xml::parseDirection(const pugi::xml_attribute& attrib, const TileElementDirection defVal)
    {
        const QString strValue = attrib.as_string("");
        if (strValue.isEmpty())
            return defVal;

        // We ignore case here.
        const auto strValueLower = strValue.toLower();

        if (strValueLower == "forward")
            return TileElementDirection::Forward;
        else if (strValueLower == "backward")
            return TileElementDirection::Backward;
        else if (strValueLower == "bidirectional")
            return TileElementDirection::Bidirectional;
        else
            return defVal;
    }
}
