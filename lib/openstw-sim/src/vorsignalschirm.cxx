#include "vorsignalschirm.hxx"
#include <stdexcept>

namespace Openstw::Simulation
{
    VorSignalSchirm::VorSignalSchirm(bool hasKennLicht) : m_hasKennLicht{hasKennLicht}
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

    void VorSignalSchirm::setHauptSignalBild(const HauptSignalBild bild)
    {
        throw std::runtime_error("Not implemented vor Vorsignal");
    }

    void VorSignalSchirm::setVorSignalBild(const VorSignalBild bild)
    {
        this->m_vorSignalBild = bild;
    }

    void VorSignalSchirm::setRangierSignalBild(const RangierSignalBild bild)
    {
        throw std::runtime_error("Not implemented vor Vorsignal");
    }

    void VorSignalSchirm::setZugDeckungsSignalBild(const ZugDeckungsSignalBild bild)
    {
        throw std::runtime_error("Not implemented vor Vorsignal");
    }

    void VorSignalSchirm::setKennLichtState(const KennLichtState state)
    {
        if (!this->m_hasKennLicht)
            throw std::runtime_error("This Vorsignal does not have a Kennlich");

        this->m_kennLichtState = state;
    }
}
