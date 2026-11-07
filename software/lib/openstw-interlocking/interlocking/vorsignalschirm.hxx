#pragma once

#include "hauptsignalschirm.hxx"
#include "signalschirm.hxx"
#include "vorsignalbild.hxx"
#include <optional>

namespace Openstw::Interlocking
{
    // A Signalschirm representing a Vorsignal.
    // Every Vorsignal requires a reference to a Hauptsignalschirm,
    // from which it derives its aspect.
    class VorSignalSchirm : public SignalSchirm
    {
        Q_OBJECT

    public:
        explicit VorSignalSchirm(SignalMast* mast, QObject* parent = nullptr);

    public:
        // The current Signalbild.
        VorSignalBild signalBild() const;

        // The main signal this distant signal currently refers to,
        // if any.
        std::optional<SignalMast*> hauptSignal() const;

        // Whether the signal is currently blanked.
        bool isBlanked() const;

    public:
        // Associate this Vorsignalschirm with given main signal.
        // It will derive its aspect from that main signal.
        void associateWith(SignalMast* mainSignal);

        // Revoke an existing
        void revokeAssociation();

        // Set whether the signal is currently blanked or not.
        void setIsBlanked(bool isBlanked);

    signals:
        // Emitted when the displayed Signalbild changes.
        void signalBildChanged(Openstw::Interlocking::VorSignalBild signalBild);

    protected slots:
        // React to the aspect of the associated main signal changing.
        void hauptSignalBildChanged(HauptSignalBild signalBild);

    protected:
        // Derive the aspect of this Vorsignal from the currently associated
        // main signal, while respecting overrides such as Dunkelschaltung.
        void deriveSignalBild();

    protected:
        // The current aspect this Vorsignal is showing.
        VorSignalBild m_signalBild{VorSignalBild::HaltErwarten};

        // The Hauptsignalschirm this Vorsignal is currently refering to.
        // This can change depending on the current route this Vorsignal
        // is involved with, and might also not be set if no route was ever set.
        std::optional<HauptSignalSchirm*> m_hauptSignalSchirm{};

        // The Signal mast containing the main signal this Vorsignal is
        // refering to.
        std::optional<SignalMast*> m_hauptSignal;

        // Whether the Vorsignalschirm is currently blanked out (Dunkelschaltung)
        bool m_isBlanked{false};
    };
}
