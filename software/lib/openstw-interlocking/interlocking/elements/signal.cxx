#include "signal.hxx"
#include <QVector>

namespace Openstw::Interlocking
{
    Signal::Signal(ElementType type, QStringView identifier, QSpan<const LockingReason> supportedLockingReasons,
                   bool supportsRouteStart, QObject* parent)
        : LockableElement{type, identifier, supportedLockingReasons, parent}, m_supportsRouteStart{supportsRouteStart}
    {
    }

    bool Signal::supportsRouteStart() const
    {
        return this->m_supportsRouteStart;
    }
}