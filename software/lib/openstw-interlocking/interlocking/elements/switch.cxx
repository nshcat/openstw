#include "switch.hxx"
#include "logging.hxx"

namespace Openstw::Interlocking
{
    Switch::Switch(QStringView identifier, Topology::SimpleSwitch* topoNode, QObject* parent)
        : LockableElement{
              ElementType::Switch, identifier, {LockingReason::FlankProtection, LockingReason::Route}, parent},
          m_topoNode{topoNode}
    {
    }

    SwitchPosition Switch::currentPosition() const
    {
        return this->m_currentPosition;
    }

    SwitchPosition Switch::targetPosition() const
    {
        return this->m_targetPosition;
    }

    bool Switch::isAdjusting() const
    {
        return this->m_isAdjusting;
    }

    Topology::SimpleSwitch* Switch::topologyNode() const
    {
        return this->m_topoNode;
    }

    bool Switch::requestPosition(SwitchPosition targetPosition)
    {
        // The position of a locked switch may not be
        // changed.
        if (this->locked())
        {
            // clang-format off
            qCFatal(catInterlocking)
                << "Can't change switch '"
                << this->identifier()
                << "' to position '"
                << targetPosition
                << "': Switch is locked"
                << Qt::endl;
            // clang-format on

            return false;
        }

        this->m_targetPosition = targetPosition;
        this->m_isAdjusting = true;

        emit this->stateChanged(this->m_currentPosition, this->m_targetPosition, this->m_isAdjusting);

        return true;
    }

    void Switch::reachPosition()
    {
        // Check that the switch is currently in the processing
        // of adjusting itself to a new target position.
        if (!this->isAdjusting())
        {
            // clang-format off
            qCFatal(catInterlocking)
                << "Switch '"
                << this->identifier()
                << "' has already reached target position '"
                << this->targetPosition()
                << "'"
                << Qt::endl;
            // clang-format on

            return;
        }

        this->m_currentPosition = this->m_targetPosition;
        this->m_isAdjusting = false;

        emit this->stateChanged(this->m_currentPosition, this->m_targetPosition, this->m_isAdjusting);

        return;
    }
}
