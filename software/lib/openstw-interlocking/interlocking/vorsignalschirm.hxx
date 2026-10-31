#pragma once

#include "hauptsignalschirm.hxx"
#include "vorsignalbild.hxx"
#include <QObject>

namespace Openstw::Interlocking
{
    // A Signalschirm representing a Vorsignal.
    // Every Vorsignal requires a reference to a Hauptsignalschirm,
    // from which it derives its aspect.
    class VorSignalSchirm : public QObject
    {
        Q_OBJECT

    public:
        explicit VorSignalSchirm(HauptSignalSchirm* hauptSignal, QObject* parent = nullptr);

    public:
        // The current Signalbild.
        VorSignalBild signalBild() const;

    signals:
        // Emitted when the displayed Signalbild changes.
        void signalBildChanged(Openstw::Interlocking::VorSignalBild signalBild);

    protected:
        // Derive the aspect of this Vorsignal from given Hauptsignal aspect.
        void deriveSignalBild(HauptSignalBild hauptSignalState);

    protected:
        // The current aspect this Vorsignal is showing.
        VorSignalBild m_signalBild{VorSignalBild::HaltErwarten};

        // The Hauptsignal this Vorsignal is connected to.
        HauptSignalSchirm* m_hauptSignal;
    };
}
