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
        explicit SignalMast(QStringView identifier, QObject* parent = nullptr);

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

        virtual std::optional<Signal*> referencedSignal() const override;

    public:
        // Set the haupt signal for this mast.
        void setHauptSignalSchirm(HauptSignalSchirm* hauptSignal);

        // Set the vor signal for this mast.
        void setVorSignalSchirm(VorSignalSchirm* vorSignal);

    protected slots:
        // React to the aspect displayed by our Hauptsignalschirm changing
        void handleHauptSignalBildChanged(HauptSignalBild newSignalBild);

    protected:
        std::optional<HauptSignalSchirm*> m_hauptSignal{};
        std::optional<VorSignalSchirm*> m_vorSignal{};
    };
}