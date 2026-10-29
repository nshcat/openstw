#include "displayelement.hxx"

namespace Openstw::SpDr
{
    DisplayElement::DisplayElement(const QString& identifier, const QVector<LampPosition>& lamps, QObject* parent)
        : QObject{parent}, m_identifier{identifier}, m_lamps{lamps}
    {
    }

    const ElementState& DisplayElement::state() const
    {
        return this->m_currentState;
    }

    bool DisplayElement::isOff() const
    {
        return this->state().isOff();
    }

    ElementColor DisplayElement::color() const
    {
        return this->state().color();
    }

    BlinkMode DisplayElement::blinkMode() const
    {
        return this->state().blinkMode();
    }

    QStringView DisplayElement::identifier() const
    {
        return {this->m_identifier};
    }

    QSpan<const LampPosition> DisplayElement::lamps() const
    {
        return {this->m_lamps};
    }

    void DisplayElement::updateState(const ElementState& newState)
    {
        // Check if the state is indeed different from our current state.
        if (newState != this->state())
        {
            // If so, apply it.
            this->m_currentState = newState;

            // Fire signal to notify our clients of the updated state.
            emit this->stateChanged(this->m_currentState);
        }
    }
}
