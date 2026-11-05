#pragma once

#include "elements/signalmast.hxx"
#include <QObject>

namespace Openstw::Interlocking
{
    // Base class for Signalschirme that are mounted to a
    // Signalmast
    class SignalSchirm : public QObject
    {
        Q_OBJECT

    public:
        // Construct new signal schirm, associated given Signalmast with it.
        explicit SignalSchirm(SignalMast* signalMast, QObject* parent = nullptr);

    public:
        // Retrieve Signalmast this Signalschirm is mounted on.
        SignalMast* mast() const;

    protected:
        // The Signalmast this Signalschirm is mounted on.
        SignalMast* m_mast;
    };
}