#include "tile.hxx"

namespace Openstw::Simulation
{
    Tile::Tile()
    {
    }

    bool Tile::hasSignal() const
    {
        return this->m_signal.has_value();
    }

    Signal& Tile::signal()
    {
        return this->m_signal.value();
    }

    bool Tile::hasTrack() const
    {
        return m_hasTrack;
    }

    void Tile::setHasTrack(bool newHasTrack)
    {
        m_hasTrack = newHasTrack;
    }

    TrackState Tile::trackState() const
    {
        return m_trackState;
    }

    void Tile::setTrackState(TrackState newTrackState)
    {
        m_trackState = newTrackState;
    }

    Tile::changed_event_t& Tile::on_changed()
    {
        return this->m_evtChanged;
    }
}
