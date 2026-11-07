#pragma once

#include "elements/tracksection.hxx"
#include "hauptsignalbild.hxx"
#include "signalschirm.hxx"

namespace Openstw::Interlocking
{
    // A Signalschirm representing a Hauptsignal.
    class HauptSignalSchirm : public SignalSchirm
    {
        Q_OBJECT

    public:
        // Construct Hauptsignalschirm mounted on given Mast, guarding given track section.
        explicit HauptSignalSchirm(SignalMast* mast, QObject* parent = nullptr);

    public:
        // The current Signalbild.
        HauptSignalBild signalBild() const;

    public:
        // Change the current Signalbild to given value.
        void setSignalBild(HauptSignalBild signalBild);

    signals:
        // Emitted when the displayed Signalbild changes.
        void signalBildChanged(Openstw::Interlocking::HauptSignalBild signalBild);

    protected:
        // The current Signalbild.
        HauptSignalBild m_signalBild{HauptSignalBild::Halt};
    };
}
