#include "erlaubnisfeld.hxx"

namespace Openstw::Simulation
{
    ErlaubnisFeld::ErlaubnisFeld()
    {
    }

    ErlaubnisFeld ErlaubnisFeld::CreateFrom(const pugi::xml_node& node)
    {
        ErlaubnisFeld erlaubnisFeld{};

        const QString ausfahrLabel = node.attribute("ausfahrLabel").as_string("");
        erlaubnisFeld.m_ausfahrLabel = ausfahrLabel;

        const QString ausfahrDirectionTxt = node.attribute("ausfahrDirection").as_string("left");
        TileElementDirection ausfahrDirection{TileElementDirection::Backward};
        if (ausfahrDirectionTxt == "right")
            ausfahrDirection = TileElementDirection::Forward;
        erlaubnisFeld.m_ausfahrtDirection = ausfahrDirection;

        return erlaubnisFeld;
    }

    const QString& ErlaubnisFeld::ausfahrRichtungLabel() const
    {
        return this->m_ausfahrLabel;
    }

    TileElementDirection ErlaubnisFeld::ausfahrRichtung() const
    {
        return this->m_ausfahrtDirection;
    }

    AusfahrErlaubnisState ErlaubnisFeld::erlaubnisState() const
    {
        return this->m_erlaubnisState;
    }
}
