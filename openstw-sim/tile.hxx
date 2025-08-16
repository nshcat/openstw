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
        bool has_signal() const;
        Signal& signal();

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
    };
}
