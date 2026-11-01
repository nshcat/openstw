#pragma once

#include "../elements/signal.hxx"
#include "../request.hxx"

namespace Openstw::Interlocking
{
    // Request to establish a Zugfahrstrasse between two signals.
    class SetZugStrasseRequest : public Request
    {
        Q_OBJECT

    public:
        SetZugStrasseRequest(Signal* from, Signal* to, QObject* parent = nullptr);

    public:
        // Signal where the route is supposed to be begin.
        Signal* fromSignal() const;

        // Signal where the route is supposed to be end.
        Signal* toSignal() const;

    protected:
        // Signal where the route is supposed to be begin.
        Signal* m_fromSignal;

        // Signal where the route is supposed to be end.
        Signal* m_toSignal;
    };
}