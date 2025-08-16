#include "signal.hxx"

namespace Openstw::Simulation
{
    Signal::Signal()
    {
    }

    bool Signal::isBlockSignal() const
    {
        return m_isBlockSignal;
    }

    BlockSignalType Signal::blockSignalType() const
    {
        return m_blockSignalType;
    }

    bool Signal::hasHauptSignal() const
    {
        return m_hasHauptSignal;
    }

    HauptSignalBild Signal::hauptSignalBild() const
    {
        return m_hauptSignalBild;
    }
}
