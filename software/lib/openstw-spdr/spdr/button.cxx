#include "button.hxx"

namespace Openstw::SpDr
{
    Button::Button(QStringView identifier, ButtonRole role, std::optional<QStringView> reference, QObject* parent)
        : QObject{parent}, m_identifier{identifier}, m_role{role}, m_reference{reference}
    {
    }

    QStringView Button::identifier() const
    {
        return this->m_identifier;
    }

    ButtonRole Button::role() const
    {
        return this->m_role;
    }

    std::optional<QStringView> Button::reference() const
    {
        return {this->m_reference};
    }

    ButtonState Button::state() const
    {
        return this->m_state;
    }

    void Button::setState(ButtonState newState)
    {
        this->m_state = newState;

        emit this->stateChanged(newState);
    }
}
