#include "hauptsignalschirm.hxx"
#include "logging.hxx"

namespace Openstw::Interlocking
{
    HauptSignalSchirm::HauptSignalSchirm(SignalMast* mast, TrackSection* followingSection, QObject* parent)
        : SignalSchirm{mast, parent}, m_followingSection{followingSection}
    {
        // Setup signal handler for reacting to the section guarded by this signal
        // becoming occupied.
        // We need to fall back to Halt whenever a train enters it!
        // clang-format off
        connect(this->m_followingSection, &TrackSection::occupancyChanged,
                this, &HauptSignalSchirm::handleSectionOccupancyChanged);
        // clang-format on
    }

    HauptSignalBild HauptSignalSchirm::signalBild() const
    {
        return this->m_signalBild;
    }

    TrackSection* HauptSignalSchirm::followingSection() const
    {
        return this->m_followingSection;
    }

    void HauptSignalSchirm::setSignalBild(HauptSignalBild signalBild)
    {
        this->m_signalBild = signalBild;

        // Notify our clients of the change.
        emit this->signalBildChanged(this->m_signalBild);
    }

    void HauptSignalSchirm::handleSectionOccupancyChanged(bool isOccupied)
    {
        // If we are currently allowing proceeding, we need to watch the
        // associated following track section for occupancy.
        // If a train passing this signal occupies that track section,
        // we need to fall back to halt in order to protect the train from
        // Auffahrunfälle.

        // Nothing to do if the following section is not occupied.
        if (!isOccupied)
        {
            return;
        }

        // Check whether we are currently displaying something other than
        // Halt.
        if (this->signalBild() != HauptSignalBild::Halt)
        {
            // clang-format off
            qCDebug(catInterlocking)
                << "Setting Hauptsignalschirm on '"
                << this->mast()->identifier()
                << "' to Halt: Following track section '"
                << this->followingSection()->identifier()
                << "' was occupied"
                << Qt::endl;
            // clang-format on

            this->setSignalBild(HauptSignalBild::Halt);
        }
    }
}
