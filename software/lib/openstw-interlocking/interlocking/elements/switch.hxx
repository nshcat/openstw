#pragma once

#include "../switchposition.hxx"
#include "lockableelement.hxx"
#include <QObject>
#include <topo/nodes/simpleswitch.hxx>

namespace Openstw::Interlocking
{
    // A simple, two-way switch.
    class Switch : public LockableElement
    {
        Q_OBJECT

    public:
        explicit Switch(QStringView identifier, Topology::SimpleSwitch* topoNode, QObject* parent = nullptr);

    public:
        // The current position of the switch.
        SwitchPosition currentPosition() const;

        // The target position of the switch.
        // This can differ from the current position if the switch is
        // currently adjusting itself.
        SwitchPosition targetPosition() const;

        // Whether the switch is currently adjusting itself to a new target position.
        bool isAdjusting() const;

        // The switch node within the topology graph this switch corresponds to.
        Topology::SimpleSwitch* topologyNode() const;

    public:
        // Attempt to change the switch position to given value.
        // Returns true if the operation succeeded. Changing the
        // position of a locked switch is not allowed.
        // This will set the target position to given position, and
        // cause the switch to start adjusting itself.
        // XXXTODO someday, this should return a process, because this operation
        // takes time!
        bool requestPosition(SwitchPosition targetPosition);

        // Causes the switch to finish its adjustment, and assume the target position.
        // XXXTODO this is just a placeholder for now..
        void reachPosition();

    signals:
        // Emitted when the current state of the switch changes.
        void stateChanged(SwitchPosition currentPosition, SwitchPosition targetPosition, bool isAdjusting);

    protected:
        // The switch node inside the topology graph this switch
        // corresponds to.
        Topology::SimpleSwitch* m_topoNode;

        // The current position of the switch.
        SwitchPosition m_currentPosition{SwitchPosition::Straight};

        // The target position of the switch. If the switch is at rest,
        // this is always the same as the current position value.
        SwitchPosition m_targetPosition{SwitchPosition::Straight};

        // Whether the switch is currently busy adjusting itself to a new
        // position.
        bool m_isAdjusting{false};
    };
}
