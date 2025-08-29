#include "tracksegment.hxx"

namespace Openstw::Simulation
{
    TrackSegment::TrackSegment()
    {
    }

    TrackSegment TrackSegment::CreateFrom(const pugi::xml_node&)
    {
        // XXX Nothing to deserialize yet..
        return TrackSegment{};
    }

    TrackState TrackSegment::state() const
    {
        return this->m_state;
    }

    void TrackSegment::setState(TrackState newState)
    {
        this->m_state = newState;
    }
}
