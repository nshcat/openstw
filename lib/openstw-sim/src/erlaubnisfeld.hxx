#pragma once

#include <pugixml.hpp>
#include <qstring.h>

#include "enums.hxx"

namespace Openstw::Simulation
{
    class ErlaubnisFeld
    {
    protected:
        ErlaubnisFeld();

    public:
        ~ErlaubnisFeld() = default;

        // No copy allowed
        ErlaubnisFeld(const ErlaubnisFeld&) = delete;
        ErlaubnisFeld& operator=(const ErlaubnisFeld&) = delete;

        ErlaubnisFeld(ErlaubnisFeld&&) = default;
        ErlaubnisFeld& operator=(ErlaubnisFeld&&) = default;

    public:
        static ErlaubnisFeld CreateFrom(const pugi::xml_node&);

    public:
        const QString& ausfahrRichtungLabel() const;
        TileElementDirection ausfahrRichtung() const;
        AusfahrErlaubnisState erlaubnisState() const;
        StaticLampState ausfahrSperrMelderState() const;

    protected:
        QString m_ausfahrLabel{""};
        TileElementDirection m_ausfahrtDirection{TileElementDirection::Forward};
        AusfahrErlaubnisState m_erlaubnisState{AusfahrErlaubnisState::ThisStellwerk};
        StaticLampState m_ausfahrSperrMelderState{StaticLampState::Off};
    };
}
