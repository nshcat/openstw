#include "switch.hxx"
#include "logging.hxx"

namespace Openstw::Interlocking
{

    Switch::Switch(QStringView identifier, QObject* parent)
        : LockableElement{
              ElementType::Switch, identifier, {LockingReason::FlankProtection, LockingReason::Route}, parent}
    {
    }

    SwitchPosition Switch::position() const
    {
        return this->m_position;
    }

    bool Switch::setPosition(SwitchPosition targetPosition)
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

        this->m_position = targetPosition;

        emit this->positionChanged(this->m_position);

        return true;
    }
}
