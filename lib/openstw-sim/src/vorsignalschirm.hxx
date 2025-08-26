#pragma once

#include "isignalschirm.hxx"

namespace Openstw::Simulation
{
    class VorSignalSchirm : public Openstw::Simulation::ISignalSchirm
    {
    public:
        VorSignalSchirm();

    public: // == ISignalSchirm interface implementation
        virtual Openstw::Simulation::FlagField<SignalBildType> supportedSignalBilder() const override;
        virtual HauptSignalBild hauptSignalBild() const override;
        virtual VorSignalBild vorSignalBild() const override;
        virtual RangierSignalBild rangierSignalBild() const override;
        virtual ZugDeckungsSignalBild zugDeckungsSignalBild() const override;
        virtual bool hasKennLicht() const override;
        virtual KennLichtState kennLichtState() const override;

    protected:
        VorSignalBild m_vorSignalBild{VorSignalBild::Off};
        bool m_hasKennLicht{false};
        KennLichtState m_kennLichtState{KennLichtState::Off};
    };
}
