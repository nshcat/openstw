#pragma once

#include "../element.hxx"
#include "axlecounterlink.hxx"
#include <QObject>
#include <QSpan>
#include <QStringView>
#include <QVector>

namespace Openstw::Interlocking
{
    // A track section, managing occupancy state for a subsection
    // of the track (and switch) network via axle counters.
    // It references a set of axle counters, with a counting
    // direction associated with each one of them (some axle counters
    // point into the section, and some point out of the section)
    class TrackSection : public Element
    {
        Q_OBJECT

    public:
        explicit TrackSection(QStringView identifier, QSpan<AxleCounterLink*> axleCounters, QObject* parent = nullptr);
        virtual ~TrackSection() = default;

    public:
        // Whether this track section is currently occupied.
        bool occupied() const;

        // Whether this track section is currently locked because
        // it is part of a Fahrstraße.
        bool locked() const;

        // The current axle counter value associated with this section.
        int counterValue() const;

        // All axle counter links associated with this section.
        QSpan<AxleCounterLink const* const> links() const;

    public:
        // Lock this track section, as part of a Fahrstraße.
        void lock();

        // Unlock this track section.
        void unlock();

    signals:
        // Emitted whenever the occupancy state of this track section changes.
        void occupancyChanged(bool isOccupied);

        // Emitted whenever the locked state of this track section changes.
        void lockedChanged(bool isLocked);

        // Emitted whenever the value of the counter kept by this track section changes.
        void counterValueChanged(int counterValue);

    protected slots:
        // Called whenever one of our associated axle counter links
        // is triggerd by a train traversing it.
        void onLinkTriggered(Openstw::Interlocking::AxleCounterOperation operation);

    protected:
        // Value for counting axles entering and exiting the section
        // by reacting to axle counter traversal events.
        int m_counterValue{0};

        // All links to axle counters that can influence the occupancy
        // state of this section.
        QVector<AxleCounterLink*> m_axleCounterLinks;

        // Whether this section is currently occupied by a train.
        bool m_isOccupied{false};

        // Whether this section is currently locked as part of a
        // Fahrstraße.
        bool m_isLocked{false};
    };
}
