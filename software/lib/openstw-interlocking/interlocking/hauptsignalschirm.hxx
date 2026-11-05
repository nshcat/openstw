#pragma once

#include "elements/tracksection.hxx"
#include "hauptsignalbild.hxx"
#include "signalschirm.hxx"

namespace Openstw::Interlocking
{
    // A Signalschirm representing a Hauptsignal.
    class HauptSignalSchirm : public SignalSchirm
    {
        Q_OBJECT

    public:
        // Construct Hauptsignalschirm mounted on given Mast, guarding given track section.
        explicit HauptSignalSchirm(SignalMast* mast, TrackSection* followingSection, QObject* parent = nullptr);

    public:
        // The current Signalbild.
        HauptSignalBild signalBild() const;

        // The track section this signal guards.
        // Whenever this section is occupied, this signals movement
        // authority will revoke itself.
        TrackSection* followingSection() const;

    public:
        // Change the current Signalbild to given value.
        void setSignalBild(HauptSignalBild signalBild);

    signals:
        // Emitted when the displayed Signalbild changes.
        void signalBildChanged(Openstw::Interlocking::HauptSignalBild signalBild);

    protected slots:
        // React to the occupancy state of the following track section changing,
        // by potentially resetting our Signalbild.
        void handleSectionOccupancyChanged(bool isOccupied);

    protected:
        // The current Signalbild.
        HauptSignalBild m_signalBild{HauptSignalBild::Halt};

        // The track section guarded by this Hauptsignalschirm.
        TrackSection* m_followingSection;
    };
}
