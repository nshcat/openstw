#pragma once

#include <boost/signals2.hpp>

namespace Openstw::Simulation
{
    class Tile
    {
    public:
        using changed_signal_t = boost::signals2::signal<void()>;

    public:
        Tile();

    public: // == Public interface
    public: // == Signals
        /**
         * Called whenever the tiles contents were changed.
         *
         * Clients should invalidate and redraw their tile represenation
         * upon receiving this signal.
         */
        auto on_changed() -> changed_signal_t&
        {
            return this->m_sigChanged;
        }

    protected:
        changed_signal_t m_sigChanged{};
    };
}
