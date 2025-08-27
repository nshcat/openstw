#include "tile.hxx"

namespace Openstw::Simulation
{
    Tile::Tile()
    {
    }

    bool Tile::hasSignal(const TileElementDirection direction) const
    {
        if (direction == TileElementDirection::Forward)
            return this->m_forwardSignal.has_value();
        else
            return this->m_backwardSignal.has_value();
    }

    Signal& Tile::signal(const TileElementDirection direction)
    {
        auto& signal = (direction == TileElementDirection::Forward) ? this->m_forwardSignal : this->m_backwardSignal;

        if (!signal.has_value())
            throw std::runtime_error("Tile doesnt contain signal in given direction");

        return signal.value();
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

    void Tile::setPosition(const GridPosition& newPosition)
    {
        this->m_position = newPosition;
    }

    const GridPosition& Tile::position() const
    {
        return this->m_position;
    }

    Tile::changed_event_t& Tile::on_changed()
    {
        return this->m_evtChanged;
    }
}
