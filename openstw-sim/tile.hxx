#pragma once

#include <optional>

#include <boost/signals2.hpp>

#include <signal.hxx>

namespace Openstw::Simulation
{
    class Tile
    {
    public:
        using changed_event_t = boost::signals2::signal<void()>;

    public: 
        Tile();

    public: // == Public interface
        bool hasSignal() const;
        Signal& signal();
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
        changed_event_t m_evtChanged{};

    protected:
        std::optional<Signal> m_signal{}; //< Signal in this tile. Can be a combination of Haupt- and Vorsignal,
                                          //  and can include a Rangiersignal.

        // XXX The following are just placeholder members to test out the
        // the architecture of the rendering system
        bool m_hasTrack{false};
        TrackState m_trackState{TrackState::Inactive};
    };
}
