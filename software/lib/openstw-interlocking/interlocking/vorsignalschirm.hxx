#pragma once

#include "hauptsignalschirm.hxx"
#include "signalschirm.hxx"
#include "vorsignalbild.hxx"

namespace Openstw::Interlocking
{
    // A Signalschirm representing a Vorsignal.
    // Every Vorsignal requires a reference to a Hauptsignalschirm,
    // from which it derives its aspect.
    class VorSignalSchirm : public SignalSchirm
    {
        Q_OBJECT

    public:
        explicit VorSignalSchirm(SignalMast* mast, SignalMast* mainSignal, QObject* parent = nullptr);

    public:
        // The current Signalbild.
        VorSignalBild signalBild() const;

        // The main signal this distant signal refers to.
        SignalMast* hauptSignal() const;

    signals:
        // Emitted when the displayed Signalbild changes.
        void signalBildChanged(Openstw::Interlocking::VorSignalBild signalBild);

    protected:
        // Derive the aspect of this Vorsignal from given Hauptsignal aspect.
        void deriveSignalBild(HauptSignalBild hauptSignalState);

    protected:
        // The current aspect this Vorsignal is showing.
        VorSignalBild m_signalBild{VorSignalBild::HaltErwarten};

        // The Hauptsignalschirm this Vorsignal is refering to.
        HauptSignalSchirm* m_hauptSignalSchirm;

        // The Signal mast containing the main signal this Vorsignal is
        // refering to.
        SignalMast* m_hauptSignal;
    };
}
