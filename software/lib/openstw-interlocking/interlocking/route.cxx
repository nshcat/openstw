#include "route.hxx"
#include <algorithm>
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    Route::Route(QSpan<const RouteSegment> segments, QObject* parent)
        : QObject{parent}
    {
        // == Validate route makeup.

        // A route must contain at least two elements (start and end signal)
        if (segments.size() < 2)
        {
            throw Core::ArgumentError("Route must contain at least two segments");
        }

        // A route must begin on a signal.
        const auto& startSegment = segments.front();

        if (!startSegment.isSignal())
        {
            throw Core::ArgumentError("Route must begin at a signal");
        }

        auto* startSignal = startSegment.asSignal();

        // The start signal must also support starting routes on it.
        if (!startSignal->supportsRouteStart())
        {
            throw Core::ArgumentError("Route start signal does not support being the start of a route");
        }

        // A route must end on a signal.
        const auto& endSegment = segments.back();

        if (!endSegment.isSignal())
        {
            throw Core::ArgumentError("Route must end at a signal");
        }

        auto* endSignal = endSegment.asSignal();

        // Verify that no other signal is part of the route.
        if (segments.size() > 2)
        {
            // clang-format off
            const auto hasFurtherSignals
                = std::any_of(  std::next(segments.begin()), std::prev(segments.end()),
                                [](const auto& segment) -> bool
                                {
                                    return segment.isSignal();
                                });
            // clang-format on

            if (hasFurtherSignals)
            {
                throw Core::ArgumentError("Route must only contain exactly two signals");
            }
        }
        // ==

        // Store segments within route, and remember references to
        // the beginning and end signals.
        this->m_segments = {segments.begin(), segments.end()};
        this->m_beginSignal = startSignal;
        this->m_endSignal = endSignal;
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

    QSpan<const RouteSegment> Route::segments() const
    {
        return {this->m_segments};
    }

    Signal* Route::beginSignal() const
    {
        return this->m_beginSignal;
    }

    Signal* Route::endSignal() const
    {
        return this->m_endSignal;
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
