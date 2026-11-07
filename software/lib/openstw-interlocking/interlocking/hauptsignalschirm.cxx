#include "hauptsignalschirm.hxx"
#include "logging.hxx"

namespace Openstw::Interlocking
{
    HauptSignalSchirm::HauptSignalSchirm(SignalMast* mast, QObject* parent)
        : SignalSchirm{mast, parent}
    {
    }

    HauptSignalBild HauptSignalSchirm::signalBild() const
    {
        return this->m_signalBild;
    }

    void HauptSignalSchirm::setSignalBild(HauptSignalBild signalBild)
    {
        // Only apply if its different from our current aspect.
        if (this->m_signalBild != signalBild)
        {
            this->m_signalBild = signalBild;

            // Notify our clients of the change.
            emit this->signalBildChanged(this->m_signalBild);
        }
    }
}
