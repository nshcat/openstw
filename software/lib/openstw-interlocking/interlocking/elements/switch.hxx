#pragma once

#include "lockableelement.hxx"
#include "switchposition.hxx"
#include <QObject>

namespace Openstw::Interlocking
{
    // A simple, two-way switch.
    class Switch : public LockableElement
    {
        Q_OBJECT

    public:
        explicit Switch(QStringView identifier, QObject* parent = nullptr);

    public:
        // The current position of the switch.
        SwitchPosition position() const;

    public:
        // Attempt to change the switch position to given value.
        // Returns true if the operation succeeded. Changing the
        // position of a locked switch is not allowed.
        bool setPosition(SwitchPosition targetPosition);

    signals:
        // Emitted when the current position of the switch changes.
        void positionChanged(SwitchPosition newPosition);

    protected:
        // The current position of the switch.
        SwitchPosition m_position{SwitchPosition::Straight};
    };
}
