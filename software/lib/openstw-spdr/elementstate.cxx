#include "elementstate.hxx"

namespace Openstw::SpDr
{
    ElementState::ElementState(ElementColor color, BlinkMode blinking)
        : m_color{color}, m_blinkMode{blinking}
    {
    }

    ElementState ElementState::off()
    {
        return ElementState{ElementColor::Off, BlinkMode::Solid};
    }

    ElementState ElementState::solid(ElementColor color)
    {
        return ElementState{color, BlinkMode::Solid};
    }

    ElementState ElementState::blinking(ElementColor color)
    {
        return ElementState{color, BlinkMode::Blinking};
    }

    ElementColor ElementState::color() const
    {
        return this->m_color;
    }

    BlinkMode ElementState::blinkMode() const
    {
        return this->m_blinkMode;
    }

    bool ElementState::isOff() const
    {
        return this->color() == ElementColor::Off;
    }
}
