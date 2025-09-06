#include "tracksegment.hxx"

namespace Openstw::Simulation
{
    TrackSegment::TrackSegment()
    {
    }

    TrackSegment TrackSegment::CreateFrom(const pugi::xml_node& node)
    {
        TrackSegment segment{};

        const auto hasLeuchtMelder = node.attribute("hasLeuchtMelder").as_bool(true);
        segment.m_hasLeuchtMelder = hasLeuchtMelder;

        const auto labelNode = node.child("LabelBox");
        if (labelNode)
        {
            segment.m_label = QString{labelNode.attribute("label").as_string("")};
            segment.m_showAlternativeMelder = labelNode.attribute("alternativeMarker").as_bool(true);
        }

        return segment;
    }

    TrackState TrackSegment::state() const
    {
        return this->m_state;
    }

    void TrackSegment::setState(TrackState newState)
    {
        this->m_state = newState;
    }

    bool TrackSegment::hasLeuchtMelder() const
    {
        return this->m_hasLeuchtMelder;
    }

    bool TrackSegment::hasLabel() const
    {
        return !this->m_label.isEmpty();
    }

    const QString& TrackSegment::label() const
    {
        return this->m_label;
    }

    bool TrackSegment::showAlternativeLeuchtMelder() const
    {
        return this->m_showAlternativeMelder;
    }
}
