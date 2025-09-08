#include "zugnummernanzeige.hxx"

namespace Openstw::Simulation
{
    ZugnummernAnzeige::ZugnummernAnzeige()
    {
    }

    ZugnummernAnzeige ZugnummernAnzeige::CreateFrom(const pugi::xml_node& node)
    {
        ZugnummernAnzeige anzeige{};

        // == Type of Anzeige
        const QString anzeigeTypeStr = node.attribute("type").as_string("compact");
        ZugnummernAnzeigeType anzeigeType{ZugnummernAnzeigeType::Compact};
        if (anzeigeTypeStr == "largeLeftPart")
            anzeigeType = ZugnummernAnzeigeType::LargeLeftPart;
        else if (anzeigeTypeStr == "largeRightPart")
            anzeigeType = ZugnummernAnzeigeType::LargeRightPart;
        anzeige.m_type = anzeigeType;

        // == Label
        const QString label = node.attribute("label").as_string("");
        anzeige.m_label = label;

        return anzeige;
    }

    ZugnummernAnzeigeState ZugnummernAnzeige::displayState() const
    {
        return this->m_displayState;
    }

    ZugnummernAnzeigeType ZugnummernAnzeige::anzeigeType() const
    {
        return this->m_type;
    }

    const Zugnummer& ZugnummernAnzeige::currentZugNummer() const
    {
        return this->m_currentZugNummer;
    }

    const QString& ZugnummernAnzeige::label() const
    {
        return this->m_label;
    }

    bool ZugnummernAnzeige::hasLabel() const
    {
        return !this->m_label.isEmpty();
    }
}
