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

    bool Signal::hasKennLicht() const
    {
        return m_hasKennLicht;
    }

    KennLichtState Signal::kennLichtState() const
    {
        return m_kennLichtState;
    }

    bool Signal::hasConnector() const
    {
        return m_hasConnector;
    }

    bool Signal::hasVorSignal() const
    {
        return m_hasVorSignal;
    }

    VorSignalBild Signal::vorSignalBild() const
    {
        return m_vorSignalBild;
    }

    bool Signal::hasRangierSignal() const
    {
        return m_hasRangierSignal;
    }

    RangierSignalBild Signal::rangierSignalBild() const
    {
        return m_rangierSignalBild;
    }

    const std::string& Signal::name() const
    {
        return m_signalName;
    }
}
