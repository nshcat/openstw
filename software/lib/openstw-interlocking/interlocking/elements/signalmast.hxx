#pragma once

#include "signal.hxx"
#include <QObject>

namespace Openstw::Interlocking
{
    // A signal mast containing a number of Signalschirme.
    class SignalMast : public Signal
    {
        Q_OBJECT

    public:
        explicit SignalMast(QStringView identifier, QObject* parent = nullptr);

    protected:
    };
}