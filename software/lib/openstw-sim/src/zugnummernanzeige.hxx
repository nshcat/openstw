#pragma once

#include "utility.hxx"
#include "zugnummer.hxx"
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
        Off = 0,

        Static = 1,

        LastDigitBlinking = 2,

        AllDigitsBlinking = 3
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
        const Zugnummer& currentZugNummer() const;
        const QString& label() const;
        bool hasLabel() const;

        void setCurrentZugNummer(const Zugnummer& newCurrentZugNummer);
        void setDisplayState(ZugnummernAnzeigeState newDisplayState);

    protected:
        ZugnummernAnzeigeType m_type{ZugnummernAnzeigeType::Compact};
        Zugnummer m_currentZugNummer{};
        ZugnummernAnzeigeState m_displayState{ZugnummernAnzeigeState::Off};
        QString m_label{""};
    };
}
