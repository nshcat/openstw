#pragma once

#include "imodifysignalschirm.hxx"
#include "isignalschirm.hxx"

namespace Openstw::Simulation
{
    class VorSignalSchirm : public ISignalSchirm, public IModifySignalSchirm
    {
    public:
        VorSignalSchirm(bool hasKennLicht);

    public:
        static std::unique_ptr<ISignalSchirm> CreateFrom(const pugi::xml_node&);

    public: // == ISignalSchirm interface implementation
        virtual FlagField<SignalBildType> supportedSignalBilder() const override;
        virtual HauptSignalBild hauptSignalBild() const override;
        virtual VorSignalBild vorSignalBild() const override;
        virtual RangierSignalBild rangierSignalBild() const override;
        virtual ZugDeckungsSignalBild zugDeckungsSignalBild() const override;
        virtual bool hasKennLicht() const override;
        virtual KennLichtState kennLichtState() const override;

    public: // == IModifySignalSchirm interface
        virtual void setHauptSignalBild(const HauptSignalBild bild) override;
        virtual void setVorSignalBild(const VorSignalBild bild) override;
        virtual void setRangierSignalBild(const RangierSignalBild bild) override;
        virtual void setZugDeckungsSignalBild(const ZugDeckungsSignalBild bild) override;
        virtual void setKennLichtState(const KennLichtState state) override;

    protected:
        VorSignalBild m_vorSignalBild{VorSignalBild::Off};
        bool m_hasKennLicht{false};
        KennLichtState m_kennLichtState{KennLichtState::Off};
    };
}
