#pragma once

#include "../hauptsignalschirm.hxx"
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

    protected:
        std::optional<HauptSignalSchirm*> m_hauptSignal{};
    };
}