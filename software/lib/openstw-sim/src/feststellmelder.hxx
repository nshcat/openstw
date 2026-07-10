#pragma once

#include <pugixml.hpp>

#include "enums.hxx"
#include "utility.hxx"

namespace Openstw::Simulation
{
    class FeststellMelder
    {
    protected:
        FeststellMelder() = default;

    public:
        ~FeststellMelder() = default;

        // No copy allowed
        FeststellMelder(const FeststellMelder&) = delete;
        FeststellMelder& operator=(const FeststellMelder&) = delete;

        FeststellMelder(FeststellMelder&&) = default;
        FeststellMelder& operator=(FeststellMelder&&) = default;

    public:
        static FeststellMelder CreateFrom(const pugi::xml_node&);

    public:
        TileElementDirection direction() const;
        StaticLampState state() const;

    protected:
        TileElementDirection m_direction{TileElementDirection::Forward};
        StaticLampState m_state{StaticLampState::Off};
    };
}
