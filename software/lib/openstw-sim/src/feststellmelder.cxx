#include "feststellmelder.hxx"

namespace Openstw::Simulation
{
    FeststellMelder FeststellMelder::CreateFrom(const pugi::xml_node& node)
    {
        FeststellMelder melder{};

        melder.m_direction = Xml::parseDirection(node.attribute("direction"));

        return melder;
    }

    TileElementDirection FeststellMelder::direction() const
    {
        return this->m_direction;
    }

    StaticLampState FeststellMelder::state() const
    {
        return this->m_state;
    }
}
