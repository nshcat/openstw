#pragma once

#include "enums.hxx"
#include <QString>
#include <pugixml.hpp>

namespace Openstw::Simulation
{
    class TrackSegment
    {
    protected:
        TrackSegment();

    public:
        TrackSegment(const TrackSegment&) = delete;
        TrackSegment& operator=(const TrackSegment&) = delete;

        TrackSegment(TrackSegment&&) = default;
        TrackSegment& operator=(TrackSegment&&) = default;

    public:
        static TrackSegment CreateFrom(const pugi::xml_node&);

    public:
        TrackState state() const;
        void setState(TrackState newState);
        bool hasLeuchtMelder() const;
        bool hasLabel() const;
        const QString& label() const;
        bool showAlternativeLeuchtMelder() const;

    protected:
        // XXX Needs to be able to model all kinds of track topologies - and track types,
        // perhaps even including switches
        TrackState m_state{TrackState::Inactive};
        bool m_hasLeuchtMelder{true};
        QString m_label{""};
        bool m_showAlternativeMelder{true}; //< Show leuchtmelder that would be covered by label
    };
}
