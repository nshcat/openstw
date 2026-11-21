#include <core/exception.hxx>

#include "button.hxx"

namespace Openstw::SpDr
{
    Button::Button(QStringView identifier, ButtonRole role, std::optional<Interlocking::Element*> referencedElement,
                   QObject* parent)
        : QObject{parent}, m_identifier{identifier}, m_role{role}, m_referencedElement{referencedElement}
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

    Interlocking::Element* Button::referencedElement() const
    {
        if (!this->referencesElement())
        {
            throw Core::RuntimeError("Button {} does not reference an interlocking element", this->identifier());
        }

        return this->m_referencedElement.value();
    }

    bool Button::referencesElement() const
    {
        return this->m_referencedElement.has_value();
    }

    ButtonState Button::state() const
    {
        return this->m_state;
    }

    bool Button::pressed() const
    {
        return this->m_state == ButtonState::Pressed;
    }

    bool Button::released() const
    {
        return this->m_state == ButtonState::Released;
    }

    void Button::setState(ButtonState newState)
    {
        this->m_state = newState;

        emit this->stateChanged(newState);
    }
}
