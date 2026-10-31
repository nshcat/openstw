#pragma once

#include "lockableelement.hxx"
#include <QObject>

namespace Openstw::Interlocking
{
    // Base class for signal elements within the interlocking network.
    class Signal : public LockableElement
    {
        Q_OBJECT

    public:
        explicit Signal(ElementType type, QStringView identifier, QSpan<const LockingReason> supportedLockingReasons,
                        bool supportsRouteStart, QObject* parent = nullptr);

    public:
        // Whether a route can originate from this signal.
        bool supportsRouteStart() const;

    protected:
        // Whether a route can begin at this signal.
        const bool m_supportsRouteStart;
    };
}
