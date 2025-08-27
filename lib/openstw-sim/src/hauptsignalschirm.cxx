#include <stdexcept>

#include "hauptsignalschirm.hxx"

namespace Openstw::Simulation
{
    HauptSignalSchirm::HauptSignalSchirm(bool hasRangierBild, bool hasKennLicht)
        : m_hasRangierBild{hasRangierBild}, m_hasKennLicht{hasKennLicht}
    {
    }

    std::unique_ptr<ISignalSchirm> HauptSignalSchirm::CreateFrom(const pugi::xml_node& node)
    {
        const auto hasKennLicht = (bool)node.child("KennLicht");
        const auto hasRangierBild = (bool)node.child("RangierSignal");

        return std::make_unique<HauptSignalSchirm>(hasRangierBild, hasKennLicht);
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

    void HauptSignalSchirm::setHauptSignalBild(const HauptSignalBild bild)
    {
        this->m_hauptSignalBild = bild;
    }

    void HauptSignalSchirm::setVorSignalBild(const VorSignalBild bild)
    {
        throw std::runtime_error("Not implemented for Hauptsignal");
    }

    void HauptSignalSchirm::setRangierSignalBild(const RangierSignalBild bild)
    {
        if (!this->m_hasRangierBild)
            throw std::runtime_error("This Hauptsignal does not support a Rangiersignalbild");

        this->m_rangierSignalBild = bild;
    }

    void HauptSignalSchirm::setZugDeckungsSignalBild(const ZugDeckungsSignalBild bild)
    {
        throw std::runtime_error("Not implemented for Hauptsignal");
    }

    void HauptSignalSchirm::setKennLichtState(const KennLichtState state)
    {
        if (!this->m_hasKennLicht)
            throw std::runtime_error("This Hauptsignal does not have a Kennlicht");

        this->m_kennLichtState = state;
    }
}
