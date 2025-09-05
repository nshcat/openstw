#pragma once

#include "utility.hxx"
#include <QString>
#include <pugixml.hpp>

namespace Openstw::Simulation
{
    enum class ZugnummernAnzeigeType
    {
        // Anzeige spans only a single tile and displays the whole number
        Compact = 0,

        // Anzeige spans two tiles, and this is the left part of it displaying the first three digits
        LargeLeftPart = 1,

        // Anzeige spans two tiles, and this is the right part of it displaying the last three digits
        LargeRightPart = 2
    };

    enum class ZugnummernAnzeigeState
    {
        Static = 0,

        LastDigitBlinking = 1,

        AllDigitsBlinking = 2
    };

    class ZugnummernAnzeige
    {
    protected:
        ZugnummernAnzeige();

    public:
        ~ZugnummernAnzeige() = default;

        // No copy allowed
        ZugnummernAnzeige(const ZugnummernAnzeige&) = delete;
        ZugnummernAnzeige& operator=(const ZugnummernAnzeige&) = delete;

        ZugnummernAnzeige(ZugnummernAnzeige&&) = default;
        ZugnummernAnzeige& operator=(ZugnummernAnzeige&&) = default;

    public:
        static ZugnummernAnzeige CreateFrom(const pugi::xml_node&);

    public:
        ZugnummernAnzeigeState displayState() const;
        ZugnummernAnzeigeType anzeigeType() const;
        const QString& currentZugNummer() const;
        const QString& label() const;

    protected:
        ZugnummernAnzeigeType m_type{ZugnummernAnzeigeType::Compact};
        QString m_currentZugNummer{""};
        ZugnummernAnzeigeState m_displayState{ZugnummernAnzeigeState::Static};
        QString m_label{""};
    };
}
