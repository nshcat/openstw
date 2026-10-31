#include "signalmast.hxx"

namespace Openstw::Interlocking
{
    SignalMast::SignalMast(QStringView identifier, QObject* parent)
        : Signal{ElementType::SignalMast,
                 identifier,
                 {LockingReason::RouteDestination, LockingReason::FlankProtection, LockingReason::RouteStart},
                 true,
                 parent}
    {
    }

    bool SignalMast::hasHauptSignal() const
    {
        return this->m_hauptSignal.has_value();
    }

    const HauptSignalSchirm* SignalMast::hauptSignal() const
    {
        return *this->m_hauptSignal;
    }

    HauptSignalSchirm* SignalMast::hauptSignal()
    {
        return *this->m_hauptSignal;
    }

    bool SignalMast::hasVorSignal() const
    {
        return this->m_vorSignal.has_value();
    }

    const VorSignalSchirm* SignalMast::vorSignal() const
    {
        return *this->m_vorSignal;
    }

    VorSignalSchirm* SignalMast::vorSignal()
    {
        return *this->m_vorSignal;
    }

    void SignalMast::setHauptSignal(HauptSignalSchirm* hauptSignal)
    {
        if (this->m_hauptSignal.has_value())
        {
            this->m_hauptSignal.value()->setParent(nullptr);
        }

        this->m_hauptSignal = hauptSignal;

        this->m_hauptSignal.value()->setParent(this);
    }

    void SignalMast::setVorSignal(VorSignalSchirm* vorSignal)
    {
        if (this->m_vorSignal.has_value())
        {
            this->m_vorSignal.value()->setParent(nullptr);
        }

        this->m_vorSignal = vorSignal;

        this->m_vorSignal.value()->setParent(this);
    }
}
