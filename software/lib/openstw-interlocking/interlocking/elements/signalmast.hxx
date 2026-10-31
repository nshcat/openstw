#pragma once

#include "../hauptsignalschirm.hxx"
#include "../vorsignalschirm.hxx"
#include "signal.hxx"
#include <QObject>
#include <optional>

namespace Openstw::Interlocking
{
    // A signal mast containing a number of Signalschirme.
    class SignalMast : public Signal
    {
        Q_OBJECT

    public:
        explicit SignalMast(QStringView identifier, QObject* parent = nullptr);

    public:
        // Whether this mast has a main signal.
        bool hasHauptSignal() const;

        // Retrieve Hauptsignal on this mast.
        const HauptSignalSchirm* hauptSignal() const;

        // Retrieve Hauptsignal on this mast.
        HauptSignalSchirm* hauptSignal();

        // Whether this mast has a distant signal.
        bool hasVorSignal() const;

        // Retrieve Vorsignal on this mast.
        const VorSignalSchirm* vorSignal() const;

        // Retrieve Vorsignal on this mast.
        VorSignalSchirm* vorSignal();

    public:
        // Set the haupt signal for this mast.
        void setHauptSignal(HauptSignalSchirm* hauptSignal);

        // Set the vor signal for this mast.
        void setVorSignal(VorSignalSchirm* vorSignal);

    protected:
        std::optional<HauptSignalSchirm*> m_hauptSignal{};
        std::optional<VorSignalSchirm*> m_vorSignal{};
    };
}