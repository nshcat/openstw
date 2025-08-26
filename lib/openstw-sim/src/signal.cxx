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

    bool Signal::hasConnector() const
    {
        return m_hasConnector;
    }

    const std::string& Signal::name() const
    {
        return m_signalName;
    }
}
