#include "tracksegment.hxx"

namespace Openstw::Simulation
{
    TrackSegment::TrackSegment()
    {
    }

    TrackSegment TrackSegment::CreateFrom(const pugi::xml_node& node)
    {
        TrackSegment segment{};

        const auto hasLeuchtMelder = node.attribute("hasLeuchtMelder").as_bool(true);
        segment.m_hasLeuchtMelder = hasLeuchtMelder;

        return segment;
    }

    TrackState TrackSegment::state() const
    {
        return this->m_state;
    }

    void TrackSegment::setState(TrackState newState)
    {
        this->m_state = newState;
    }

    bool TrackSegment::hasLeuchtMelder() const
    {
        return this->m_hasLeuchtMelder;
    }
}
