#include "setzugstrasserequest.hxx"

namespace Openstw::Interlocking
{
    SetZugStrasseRequest::SetZugStrasseRequest(Signal* from, Signal* to, QObject* parent)
        : Request{RequestType::SetZugStrasse, parent}, m_fromSignal{from}, m_toSignal{to}
    {
    }

    Signal* SetZugStrasseRequest::fromSignal() const
    {
        return this->m_fromSignal;
    }

    Signal* SetZugStrasseRequest::toSignal() const
    {
        return this->m_toSignal;
    }
}