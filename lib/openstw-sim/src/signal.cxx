#include "signal.hxx"

namespace Openstw::Simulation
{
    Signal::Signal()
    {
    }

    TileElementDirection Signal::direction() const
    {
        return this->m_direction;
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

    ISignalSchirm* Signal::primarySignalSchirm()
    {
        return this->m_primarySchirm.get();
    }

    bool Signal::hasSecondarySignalSchirm() const
    {
        return this->m_secondarySchirm.has_value();
    }

    VorSignalSchirm* Signal::secondarySignalSchirm()
    {
        if (!this->hasSecondarySignalSchirm())
            throw std::runtime_error("Signal has no secondary Signalschirm");

        return this->m_secondarySchirm.value().get();
    }
}
