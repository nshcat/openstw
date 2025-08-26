#include <stdexcept>

#include "hauptsignalschirm.hxx"

namespace Openstw::Simulation
{
    HauptSignalSchirm::HauptSignalSchirm()
    {
    }

    FlagField<SignalBildType> HauptSignalSchirm::supportedSignalBilder() const
    {
        FlagField<SignalBildType> supportedBilder{SignalBildType::HauptSignal};
        if (this->m_hasRangierBild)
            supportedBilder.set(SignalBildType::RangierSignal);

        return supportedBilder;
    }

    HauptSignalBild HauptSignalSchirm::hauptSignalBild() const
    {
        return this->m_hauptSignalBild;
    }

    VorSignalBild HauptSignalSchirm::vorSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Hauptsignal");
    }

    RangierSignalBild HauptSignalSchirm::rangierSignalBild() const
    {
        return this->m_rangierSignalBild;
    }

    ZugDeckungsSignalBild HauptSignalSchirm::zugDeckungsSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Hauptsignal");
    }

    bool HauptSignalSchirm::hasKennLicht() const
    {
        return this->m_hasKennLicht;
    }

    KennLichtState HauptSignalSchirm::kennLichtState() const
    {
        return this->m_kennLichtState;
    }
}
