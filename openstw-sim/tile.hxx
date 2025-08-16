#pragma once

#include <optional>

#include <boost/signals2.hpp>

#include <signal.hxx>

namespace Openstw::Simulation
{
    class Tile
    {
    public:
        using changed_signal_t = boost::signals2::signal<void()>;

    public:
        Tile();

    public: // == Public interface
        bool has_signal() const
        {
        }
        Signal& signal();

    public: // == Signals
        /**
         * Called whenever the tiles contents were changed.
         *
         * Clients should invalidate and redraw their tile representation
         * upon receiving this signal.
         */
        changed_signal_t& on_changed();

    protected:
        changed_signal_t m_sigChanged{};

    protected:
        std::optional<Signal> m_signal{}; //< Signal in this tile. Can be a combination of Haupt- and Vorsignal,
                                          //  and can include a Rangiersignal.
    };
}
