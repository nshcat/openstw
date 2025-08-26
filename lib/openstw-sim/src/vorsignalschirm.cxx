#include "vorsignalschirm.hxx"
#include <stdexcept>

namespace Openstw::Simulation
{
    VorSignalSchirm::VorSignalSchirm()
    {
    }

    FlagField<SignalBildType> VorSignalSchirm::supportedSignalBilder() const
    {
        return {SignalBildType::VorSignal};
    }

    HauptSignalBild VorSignalSchirm::hauptSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Vorsignal");
    }

    VorSignalBild VorSignalSchirm::vorSignalBild() const
    {
        return this->m_vorSignalBild;
    }

    RangierSignalBild VorSignalSchirm::rangierSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Vorsignal");
    }

    ZugDeckungsSignalBild VorSignalSchirm::zugDeckungsSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Vorsignal");
    }

    bool VorSignalSchirm::hasKennLicht() const
    {
        return this->m_hasKennLicht;
    }

    KennLichtState VorSignalSchirm::kennLichtState() const
    {
        return this->m_kennLichtState;
    }
}
