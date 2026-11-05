#include "signalschirm.hxx"

namespace Openstw::Interlocking
{
    SignalSchirm::SignalSchirm(SignalMast* signalMast, QObject* parent)
        : QObject{parent}, m_mast{signalMast}
    {
    }

    SignalMast* SignalSchirm::mast() const
    {
        return this->m_mast;
    }
}
