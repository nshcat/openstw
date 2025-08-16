#include "tile.hxx"

namespace Openstw::Simulation
{
    Tile::Tile()
    {
    }

    bool Tile::has_signal() const
    {
        return this->m_signal.has_value();
    }

    Signal& Tile::signal()
    {
        return this->m_signal.value();
    }

    Tile::changed_signal_t& Tile::on_changed()
    {
        return this->m_sigChanged;
    }
}
