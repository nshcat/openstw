#pragma once

#include "requestdenialreason.hxx"
#include "requeststate.hxx"
#include "requesttype.hxx"
#include <QObject>
#include <QUuid>
#include <optional>

namespace Openstw::Interlocking
{
    // Base class for requests aimed at the interlocking system.
    class Request : public QObject
    {
        Q_OBJECT

    public:
        explicit Request(RequestType type, QObject* parent = nullptr);

    public:
        // Unique identifier of this request.
        const QUuid& identifier() const;

        // Type of this request.
        RequestType type() const;

        // Current state of this request.
        RequestState state() const;

        // Whether the request is still pending.
        bool isPending() const;

        // Whether the request was denied.
        bool wasDenied() const;

        // If this request was denied, the reason for said denial.
        RequestDenialReason denialReason() const;

        // Whether the request was completed.
        bool wasCompleted() const;

    public:
        // Mark the request as completed.
        void complete();

        // Mark the request as denied, with given reason.
        void deny(RequestDenialReason reason);

    signals:
        // Emitted whenever the requests state changes.
        void stateChanged(RequestState newState);

        // Emitted if the request is completed.
        void completed();

        // Emitted if the request gets denied.
        void denied(RequestDenialReason reason);

    protected:
        // Unique identifier of this request.
        QUuid m_id{QUuid::createUuid()};

        // Type of this request.
        RequestType m_type;

        // Current state of this request.
        RequestState m_state{RequestState::Pending};

        // If this request was denied, the reason for said denial.
        std::optional<RequestDenialReason> m_denialReason{};
    };

}
