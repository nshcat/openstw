#include "zugdeckungssignalschirm.hxx"
#include <stdexcept>

namespace Openstw::Simulation
{
    ZugDeckungsSignalSchirm ::ZugDeckungsSignalSchirm()
    {
    }

    std::unique_ptr<ISignalSchirm> ZugDeckungsSignalSchirm::CreateFrom(const pugi::xml_node& node)
    {
        return std::make_unique<ZugDeckungsSignalSchirm>();
    }

    SignalSchirmType ZugDeckungsSignalSchirm::type() const
    {
        return SignalSchirmType::ZugDeckungsSignal;
    }

    FlagField<SignalBildType> ZugDeckungsSignalSchirm::supportedSignalBilder() const
    {
        return {SignalBildType::ZugDeckungsSignal};
    }

    HauptSignalBild ZugDeckungsSignalSchirm::hauptSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Zugdeckungssignal");
    }

    VorSignalBild ZugDeckungsSignalSchirm::vorSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Zugdeckungssignal");
    }

    RangierSignalBild ZugDeckungsSignalSchirm::rangierSignalBild() const
    {
        throw std::runtime_error("Unimplemented for Zugdeckungssignal");
    }

    ZugDeckungsSignalBild ZugDeckungsSignalSchirm::zugDeckungsSignalBild() const
    {
        return this->m_signalBild;
    }

    bool ZugDeckungsSignalSchirm::hasKennLicht() const
    {
        return true;
    }

    KennLichtState ZugDeckungsSignalSchirm::kennLichtState() const
    {
        return this->m_kennLichtState;
    }

    void ZugDeckungsSignalSchirm::setHauptSignalBild(const HauptSignalBild bild)
    {
        throw std::runtime_error("Unimplemented for Zugdeckungssignal");
    }

    void ZugDeckungsSignalSchirm::setVorSignalBild(const VorSignalBild bild)
    {
        throw std::runtime_error("Unimplemented for Zugdeckungssignal");
    }

    void ZugDeckungsSignalSchirm::setRangierSignalBild(const RangierSignalBild bild)
    {
        throw std::runtime_error("Unimplemented for Zugdeckungssignal");
    }

    void ZugDeckungsSignalSchirm::setZugDeckungsSignalBild(const ZugDeckungsSignalBild bild)
    {
        this->m_signalBild = bild;
    }

    void ZugDeckungsSignalSchirm::setKennLichtState(const KennLichtState state)
    {
        this->m_kennLichtState = state;
    }
}
