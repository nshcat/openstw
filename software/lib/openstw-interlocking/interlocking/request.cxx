#include "request.hxx"
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    Request::Request(RequestType type, QObject* parent)
        : QObject{parent}, m_type{type}
    {
    }

    const QUuid& Request::identifier() const
    {
        return this->m_id;
    }

    RequestType Request::type() const
    {
        return this->m_type;
    }

    RequestState Request::state() const
    {
        return this->m_state;
    }

    bool Request::isPending() const
    {
        return this->state() == RequestState::Pending;
    }

    bool Request::wasDenied() const
    {
        return this->state() == RequestState::Denied;
    }

    RequestDenialReason Request::denialReason() const
    {
        if (!this->wasDenied())
        {
            throw Core::RuntimeError("Request was not denied");
        }

        return *this->m_denialReason;
    }

    bool Request::wasCompleted() const
    {
        return this->state() == RequestState::Completed;
    }

    void Request::complete()
    {
        if (!this->isPending())
        {
            throw Core::RuntimeError("Can only complete pending requests");
        }

        this->m_state = RequestState::Completed;

        emit this->stateChanged(this->m_state);
        emit this->completed();
    }

    void Request::deny(RequestDenialReason reason)
    {
        if (!this->isPending())
        {
            throw Core::RuntimeError("Can only deny pending requests");
        }

        this->m_state = RequestState::Denied;
        this->m_denialReason = reason;

        emit this->stateChanged(this->m_state);
        emit this->denied(reason);
    }
}
