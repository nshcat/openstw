#pragma once

#include "imodifysignalschirm.hxx"
#include "isignalschirm.hxx"

namespace Openstw::Simulation
{
    class HauptSignalSchirm : public ISignalSchirm, public IModifySignalSchirm
    {
    public:
        HauptSignalSchirm(bool hasRangierBild, bool hasKennLicht);

    public:
        static std::unique_ptr<ISignalSchirm> CreateFrom(const pugi::xml_node&);

    public: // == ISignalSchirm interface implementation
        virtual SignalSchirmType type() const override;
        virtual FlagField<SignalBildType> supportedSignalBilder() const override;
        virtual HauptSignalBild hauptSignalBild() const override;
        virtual VorSignalBild vorSignalBild() const override;
        virtual RangierSignalBild rangierSignalBild() const override;
        virtual ZugDeckungsSignalBild zugDeckungsSignalBild() const override;
        virtual bool hasKennLicht() const override;
        virtual KennLichtState kennLichtState() const override;

    public: // == IModifySignalSchirm interface implementation
        virtual void setHauptSignalBild(const HauptSignalBild bild) override;
        virtual void setVorSignalBild(const VorSignalBild bild) override;
        virtual void setRangierSignalBild(const RangierSignalBild bild) override;
        virtual void setZugDeckungsSignalBild(const ZugDeckungsSignalBild bild) override;
        virtual void setKennLichtState(const KennLichtState state) override;

    protected:
        HauptSignalBild m_hauptSignalBild{HauptSignalBild::Hp0};
        bool m_hasRangierBild{false};
        RangierSignalBild m_rangierSignalBild{RangierSignalBild::Off};
        bool m_hasKennLicht{false};
        KennLichtState m_kennLichtState{KennLichtState::Off};
    };
}
