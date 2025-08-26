#pragma once

#include "isignalschirm.hxx"

namespace Openstw::Simulation
{
    class HauptSignalSchirm : public ISignalSchirm
    {
    public:
        HauptSignalSchirm();

    public: // == ISignalSchirm interface implementation
        virtual FlagField<SignalBildType> supportedSignalBilder() const override;
        virtual HauptSignalBild hauptSignalBild() const override;
        virtual VorSignalBild vorSignalBild() const override;
        virtual RangierSignalBild rangierSignalBild() const override;
        virtual ZugDeckungsSignalBild zugDeckungsSignalBild() const override;
        virtual bool hasKennLicht() const override;
        virtual KennLichtState kennLichtState() const override;

    protected:
        HauptSignalBild m_hauptSignalBild{HauptSignalBild::Hp0};
        bool m_hasRangierBild{false};
        RangierSignalBild m_rangierSignalBild{RangierSignalBild::Off};
        bool m_hasKennLicht{false};
        KennLichtState m_kennLichtState{KennLichtState::Off};
    };
}
