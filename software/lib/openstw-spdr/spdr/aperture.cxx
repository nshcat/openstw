#include "aperture.hxx"

namespace Openstw::SpDr
{
    Aperture::Aperture(const QString& identifier, QObject* parent)
        : QObject{parent}, m_identifier{identifier}
    {
    }

    const ApertureState& Aperture::state() const
    {
        return this->m_currentState;
    }

    bool Aperture::isOff() const
    {
        return this->state().isOff();
    }

    ApertureColor Aperture::color() const
    {
        return this->state().color();
    }

    BlinkMode Aperture::blinkMode() const
    {
        return this->state().blinkMode();
    }

    QStringView Aperture::identifier() const
    {
        return {this->m_identifier};
    }

    void Aperture::updateState(const ApertureState& newState)
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
