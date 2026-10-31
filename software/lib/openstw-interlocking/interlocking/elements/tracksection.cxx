#include "tracksection.hxx"
#include "logging.hxx"
#include <QSpan>

namespace Openstw::Interlocking
{

    TrackSection::TrackSection(QStringView identifier, QSpan<AxleCounterLink*> axleCounters, QObject* parent)
        : LockableElement{ElementType::TrackSection, identifier, {LockingReason::Route}, parent},
          m_axleCounterLinks{axleCounters.begin(), axleCounters.end()}
    {
        // Connect all triggered signals emitted from the axle counter
        // links to our handler slot.
        for (auto* link : this->m_axleCounterLinks)
        {
            // clang-format off
            connect(link, &AxleCounterLink::triggered,
                    this, &TrackSection::onLinkTriggered);
            // clang-format on

            // Mark the link object as being our child, for lifetime
            // management.
            link->setParent(this);
        }
    }

    bool TrackSection::occupied() const
    {
        return this->m_isOccupied;
    }

    int TrackSection::counterValue() const
    {
        return this->m_counterValue;
    }

    QSpan<const AxleCounterLink* const> TrackSection::links() const
    {
        return {this->m_axleCounterLinks};
    }

    void TrackSection::onLinkTriggered(AxleCounterOperation operation)
    {
        bool counterChanged{false};

        if (operation == AxleCounterOperation::CountDown)
        {
            // Sanity check
            if (this->m_counterValue == 0)
            {
                // clang-format off
                qCWarning(catInterlocking)
                    << "Track section '"
                    << this->identifier()
                    << "' encountered count down on counter value 0"
                    << Qt::endl;
                // clang-format on
            }
            else
            {
                this->m_counterValue--;
                counterChanged = true;
            }
        }
        else
        {
            this->m_counterValue++;
            counterChanged = true;
        }

        if (counterChanged)
        {
            // Determine whether we need to update the occupancy state.
            if (this->m_counterValue == 0 && this->occupied())
            {
                this->m_isOccupied = false;
                emit this->occupancyChanged(false);
            }
            else if (this->m_counterValue > 0 && !this->occupied())
            {
                this->m_isOccupied = true;
                emit this->occupancyChanged(true);
            }

            emit this->counterValueChanged(this->m_counterValue);
        }
    }
}
