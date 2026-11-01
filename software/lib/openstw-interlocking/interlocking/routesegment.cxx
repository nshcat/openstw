#include "routesegment.hxx"
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    RouteSegment::RouteSegment(Element* element, RouteSegmentType category)
        : m_element{element}, m_type{category}
    {
    }

    RouteSegment::RouteSegment(Switch* switchElement)
        : RouteSegment(switchElement, RouteSegmentType::Switch)
    {
    }

    RouteSegment::RouteSegment(Signal* signalElement)
        : RouteSegment(signalElement, RouteSegmentType::Signal)
    {
    }

    RouteSegment::RouteSegment(TrackSection* sectionElement)
        : RouteSegment(sectionElement, RouteSegmentType::TrackSection)
    {
    }

    RouteSegmentType RouteSegment::type() const
    {
        return this->m_type;
    }

    Element* RouteSegment::element() const
    {
        return this->m_element;
    }

    bool RouteSegment::isSwitch() const
    {
        return this->m_type == RouteSegmentType::Switch;
    }

    Switch* RouteSegment::asSwitch() const
    {
        if (!this->isSwitch())
        {
            throw Core::RuntimeError("Route segment with element '{}' is not a switch", this->element()->identifier());
        }

        auto* ptr = dynamic_cast<Switch*>(this->element());

        if (!ptr)
        {
            throw Core::RuntimeError("Route segment with element '{}' unexpectedly did not contain a switch",
                                     this->element()->identifier());
        }

        return ptr;
    }

    SwitchPosition RouteSegment::targetPosition() const
    {
        if (!this->isSwitch())
        {
            throw Core::RuntimeError("Route segment with element '{}' is not a switch", this->element()->identifier());
        }

        return *this->m_switchPosition;
    }

    bool RouteSegment::isSignal() const
    {
        return this->m_type == RouteSegmentType::Signal;
    }

    Signal* RouteSegment::asSignal() const
    {
        if (!this->isSignal())
        {
            throw Core::RuntimeError("Route segment with element '{}' is not a signal", this->element()->identifier());
        }

        auto* ptr = dynamic_cast<Signal*>(this->element());

        if (!ptr)
        {
            throw Core::RuntimeError("Route segment with element '{}' unexpectedly did not contain a signal",
                                     this->element()->identifier());
        }

        return ptr;
    }

    bool RouteSegment::isSection() const
    {
        return this->m_type == RouteSegmentType::TrackSection;
    }

    TrackSection* RouteSegment::asSection() const
    {
        if (!this->isSection())
        {
            throw Core::RuntimeError("Route segment with element '{}' is not a track section",
                                     this->element()->identifier());
        }

        auto* ptr = dynamic_cast<TrackSection*>(this->element());

        if (!ptr)
        {
            throw Core::RuntimeError("Route segment with element '{}' unexpectedly did not contain a track section",
                                     this->element()->identifier());
        }

        return ptr;
    }
}