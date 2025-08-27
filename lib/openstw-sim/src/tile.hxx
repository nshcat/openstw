#pragma once

#include <optional>

#include <boost/signals2.hpp>

#include <signal.hxx>
#include <utility.hxx>

namespace Openstw::Simulation
{
    class TilePanel;

    class Tile
    {
        friend class TilePanel;

    public:
        using changed_event_t = boost::signals2::signal<void()>;

    public: 
        Tile();

    public: // == Public interface
        const GridPosition& position() const;
        bool hasSignal(const TileElementDirection direction) const;
        Signal& signal(const TileElementDirection direction);
        bool hasTrack() const;
        TrackState trackState() const;

        // XXX Setters are for testing only for now
        void setHasTrack(bool newHasTrack);
        void setTrackState(TrackState newTrackState);

    public: // == Signals
        /**
         * Called whenever the tiles state was changed.
         *
         * Clients should invalidate and redraw their tile representation
         * upon receiving this signal.
         */
        changed_event_t& on_changed();

    protected:
        void setPosition(const GridPosition& newPosition);

    protected:
        changed_event_t m_evtChanged{};

    protected:
        GridPosition m_position{};
        std::optional<Signal> m_forwardSignal{};  //< Signal for traffic going left to right
        std::optional<Signal> m_backwardSignal{}; //< Signal for traffic from right to left

        // XXX The following are just placeholder members to test out the
        // the architecture of the rendering system
        bool m_hasTrack{false};
        TrackState m_trackState{TrackState::Inactive};
    };
}
