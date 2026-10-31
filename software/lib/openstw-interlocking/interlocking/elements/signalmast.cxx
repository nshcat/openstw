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
}
