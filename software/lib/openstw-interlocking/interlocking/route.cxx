#include "route.hxx"

namespace Openstw::Interlocking
{

    Route::Route(Signal* begin, Signal* end, QObject* parent)
        : QObject{parent}, m_begin{begin}, m_end{end}
    {
    }

    RouteState Route::state() const
    {
        return this->m_state;
    }

    bool Route::locked() const
    {
        return this->m_state == RouteState::Locked;
    }

    bool Route::released() const
    {
        return this->m_state == RouteState::Released;
    }

    void Route::setState(RouteState newState)
    {
        if (this->m_state != newState)
        {
            this->m_state = newState;

            emit this->stateChanged(this->m_state);
        }
    }
}
