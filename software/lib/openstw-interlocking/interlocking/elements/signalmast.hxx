#pragma once

#include "hauptsignalbild.hxx"
#include "signal.hxx"
#include <QObject>
#include <optional>

namespace Openstw::Interlocking
{
    // Forward declarations
    class HauptSignalSchirm;
    class VorSignalSchirm;

    // A signal mast containing a number of Signalschirme.
    class SignalMast : public Signal
    {
        Q_OBJECT

    public:
        explicit SignalMast(QStringView identifier, Topology::Signal* topoNode, QObject* parent = nullptr);

    public:
        // Whether this mast has a main signal.
        bool hasHauptSignalSchirm() const;

        // Retrieve Hauptsignal on this mast.
        const HauptSignalSchirm* hauptSignalSchirm() const;

        // Retrieve Hauptsignal on this mast.
        HauptSignalSchirm* hauptSignalSchirm();

        // Whether this mast has a distant signal.
        bool hasVorSignalSchirm() const;

        // Retrieve Vorsignal on this mast.
        const VorSignalSchirm* vorSignalSchirm() const;

        // Retrieve Vorsignal on this mast.
        VorSignalSchirm* vorSignalSchirm();

    public:
        virtual bool isMainSignal() const override;

        virtual bool isShuntingSignal() const override;

        virtual bool isDistantSignal() const override;

        virtual std::optional<Signal*> associatedSignal() const override;

    public:
        virtual bool hasAssociatedSignal() const override;

        virtual void associateWithSignal(Signal* mainSignal) override;

        virtual void revokeSignalAssociation() override;

    public:
        // Set the haupt signal for this mast.
        void setHauptSignalSchirm(HauptSignalSchirm* hauptSignal);

        // Set the vor signal for this mast.
        void setVorSignalSchirm(VorSignalSchirm* vorSignal);

    protected:
        // Make sure that any attached Vorsignal is correctly blanked or unblanked
        // depending on the current main signal state.
        void updateBlankingState();

    protected:
        // Handle a change of movement authority, as reported
        // by the base class.
        virtual void onMovementAuthorityChanged() override;

    protected slots:
        // Called whenever the Signalbild of our Hauptsignalschirm changes.
        // We use this to implemented automatic blanking of distant signals
        // that might be mounted on this very same mast.
        void onHauptSignalBildChanged(HauptSignalBild signalBild);

    protected:
        std::optional<HauptSignalSchirm*> m_hauptSignal{};
        std::optional<VorSignalSchirm*> m_vorSignal{};
    };
}