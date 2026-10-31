#include "aperturestate.hxx"

namespace Openstw::SpDr
{
    ApertureState::ApertureState(ApertureColor color, BlinkMode blinking)
        : m_color{color}, m_blinkMode{blinking}
    {
    }

    ApertureState ApertureState::off()
    {
        return ApertureState{ApertureColor::Off, BlinkMode::Solid};
    }

    ApertureState ApertureState::solid(ApertureColor color)
    {
        return ApertureState{color, BlinkMode::Solid};
    }

    ApertureState ApertureState::blinking(ApertureColor color)
    {
        return ApertureState{color, BlinkMode::Blinking};
    }

    ApertureColor ApertureState::color() const
    {
        return this->m_color;
    }

    BlinkMode ApertureState::blinkMode() const
    {
        return this->m_blinkMode;
    }

    bool ApertureState::isOff() const
    {
        return this->color() == ApertureColor::Off;
    }
}
